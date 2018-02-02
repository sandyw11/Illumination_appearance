///////////////////////////////
// EXR importer and exporter //
///////////////////////////////

#ifndef _BUFFER2DIO_EXR_H_
#define _BUFFER2DIO_EXR_H_

#include <vector>
#include <string>
#include <cstdlib>

#include "exceptions.h"
#include "buffer2dIO.exr.options.h"

#ifdef INCLUDE_OPENEXR

#include "offset_iterator.h"
#include "buffer2dIO.exr.detail.h"

#include "ImfHeader.h"
#include "ImfPixelType.h"
#include "half.h"
#include "ImfCompression.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfFrameBuffer.h"
#include "ImfChannelList.h"

#endif /* INCLUDE_OPENEXR */

using namespace std;

namespace io {
  namespace exr {

#ifndef INCLUDE_OPENEXR

    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float pad=0.0f, const options& opt=options()) { throw unsupportedFormat(); }

    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, float pad=0.0f, const options& opt=options()) { throw unsupportedFormat(); }

#else /* INCLUDE_OPENEXR */


    ////////////////
    // Export EXR //
    ////////////////
    template<typename Buffer, typename C>
      void _export(const string& filename, const Buffer& buf, float padding=0.0f, const options& opt=options())
    {
      // sanity check
      if(buf.width() == 0 || buf.height() == 0) throw buffer2dIllegalSize();

      // determine number of channels
      const C wrapper(*(buf.begin()));
      unsigned int storedChannels = std::distance( wrapper.begin(), wrapper.end() );
      unsigned int numChannels = (opt.hasNamedChannels()) ? opt.numberOfChannels() : storedChannels;

      // setup EXR file
      Imf::Header header(buf.width(), buf.height(), 1, Imath::V2f(0,0), 1, Imf::INCREASING_Y, Imf::Compression(opt.compressionType()));
      Imf::FrameBuffer frameBuffer;

      // allocate memory
      tempArray(char, buffer, buf.width() * buf.height() * numChannels * ::io::exr::detail::bytesPerPixel(opt.pixelType()));

      // convert
      ::io::exr::detail::convertPixelToFlat<typename Buffer::const_iterator, C>( buf.begin(), buf.end(), (void *)(&(buffer[0])), numChannels, opt.pixelType(), padding);

      // output frameBuffers
      for(unsigned int c=0; c < numChannels; c++)
      {
	// setup header
	std::string name = opt.channelName(c).c_str();
	header.channels().insert( name.c_str(), (Imf::PixelType)(opt.pixelType()) );

	// insert in EXR frame buffer
	frameBuffer.insert( name.c_str(),
			    Imf::Slice((Imf::PixelType)(opt.pixelType()),                                                // pixel types
				       &(buffer[c*::io::exr::detail::bytesPerPixel(opt.pixelType())]),                   // ptr to buffer
				       ::io::exr::detail::bytesPerPixel(opt.pixelType())*numChannels,                    // stride
				       buf.width() * ::io::exr::detail::bytesPerPixel(opt.pixelType()) * numChannels,    // bytes per scanline
				       1, 1,
				       padding) );
      }

      // write file
      Imf::OutputFile file(filename.c_str(), header);
      file.setFrameBuffer(frameBuffer);
      file.writePixels(buf.height());

      // done.
    }


    ////////////////
    // Import EXR //
    ////////////////
    template<typename Buffer, typename C>
      void _import(const string& filename, Buffer& buf, float padding=0.0f, const options& opt=options())
    {
      // check number of requested channels
      const C wrapper(*(buf.begin()));
      unsigned int availableChannels = std::distance( wrapper.begin(), wrapper.end() );
      unsigned int numChannels = availableChannels;
      if(opt.hasNamedChannels())
      {
	if(opt.numberOfChannels() > availableChannels) throw customException("import EXR: more channels requested than available in target buffer.");
	numChannels = opt.numberOfChannels();
      }

      // open exr and get relevant data
      Imf::InputFile file(filename.c_str());

      const Imath::Box2i& dataWindow = file.header().dataWindow();
      const Imf::ChannelList& chanList = file.header().channels();

      unsigned long width = dataWindow.max.x - dataWindow.min.x + 1;
      unsigned long height = dataWindow.max.y - dataWindow.min.y + 1;
      unsigned int storedChannels = 0;
      for(Imf::ChannelList::ConstIterator i = chanList.begin(); i != chanList.end(); i++) storedChannels++;

      // allocate memory
      buf.resize(width, height);

      // setup frame buffers
      Imf::FrameBuffer frameBuffer;
      std::vector< buffer2d<unsigned char> > tempBuffer;     tempBuffer.reserve(numChannels);       // avoid copying of buffers
      std::vector< ::io::exr::pixel > tempBufferType;

      std::string chanName;
      Imf::ChannelList::ConstIterator chanItr = chanList.begin();
      for(unsigned int c=0; c < numChannels; c++, chanItr++)
	{
	// get name of channel
	if(!opt.hasNamedChannels()) chanName = chanItr.name();
	else chanName = opt.channelName(c);

	// Find channel (will throw an exception if not found!)
	const Imf::Channel& chan = chanList[chanName.c_str()];

	// allocate temp storage buffer
	tempBufferType.push_back( ::io::exr::pixel(chan.type) );  
	tempBuffer.push_back( buffer2d<unsigned char>(width * ::io::exr::detail::bytesPerPixel( tempBufferType.back() ), height) );

	// setup frame buffer
	frameBuffer.insert(chanName.c_str(),
			   Imf::Slice( chan.type,                                                     // pixel type
				       (char *)(tempBuffer.back().begin()),                           // ptr to buffer
				       ::io::exr::detail::bytesPerPixel( tempBufferType.back() ),     // stride
				       tempBuffer.back().width(),                                     // bytes per scanlinle
				       1, 1, 
				       0.0));

      }

      // read file
      file.setFrameBuffer(frameBuffer);
      file.readPixels(dataWindow.min.y, dataWindow.max.y);

      // copy data to buffer (convert or pad if needed)
      for(unsigned int c=0; c < availableChannels; c++)
      {
	offset_iterator<typename Buffer::iterator> itr_begin(buf.begin(), c);
	offset_iterator<typename Buffer::iterator> itr_end(buf.end(), c);

	// convert
	if(c < numChannels)
	  ::io::exr::detail::convertFlatToPixel<offset_iterator<typename Buffer::iterator>, iteratorWrapper<typename C::value_type> >(tempBuffer[c].begin(), 1, itr_begin, itr_end, tempBufferType[c], padding);

	// pad
	else std::fill(itr_begin, itr_end, padding);
      }

      // Done.
    }
#endif /* INCLUDE_OPENEXR */


  } // end exr namespace
}   // end io  namespace

#endif /* _BUFFER2DIO_EXR_H_ */
