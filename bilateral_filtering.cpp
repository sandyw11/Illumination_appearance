#include <iostream>
#include <string>

#include "image.h"
#include "imageIO.h"

void bilateralfilter(image &original, int x, int y)
{

    float sigmad = 5.0; //
    float sigmar = 0.05; // 0.05~0.1
    int window = 3;
    float red0 = original(x, y).r;
    float green0 = original(x, y).g;
    float blue0 = original(x, y).b;
    int i, j;
    float gd;
    float red, green, blue;
    float red2 = 0.0;
    float green2 = 0.0;
    float blue2 = 0.0;
    float gr;
    float sumweight = 0.0;
    
    
    for(i = -window; i <= window; i++)
    {
        for(j = -window; j <= window; j++)
        {
            if((x + i < original.width()) && (x + i >= 0) && (y + j < original.height()) && (y + j >= 0))
            {
                gd = exp(- (float)(i * i + j * j) / (2 * sigmad * sigmad));

                red = original(x + i, y + j).r;
                green = original(x + i, y + j).g;
                blue = original(x + i, y + j).b;
            
                gr = exp(- ((red0 - red) * (red0 - red) + (green0 - green) * (green0 - green) + (blue0 - blue) * (blue0 - blue)) / (2 * sigmar * sigmar));

                sumweight += gd * gr;
		
		red2 += original(x + i, y + j).r * gd * gr;
                green2 += original(x + i, y + j).g * gd * gr;
                blue2 += original(x + i, y + j).b * gd * gr;
            }
        }
    }
    
    original(x, y).r = red2 / sumweight;
    original(x, y).g = green2 / sumweight;
    original(x, y).b = blue2 / sumweight; 
}

void jointbilateralfilter(image &original, image &flash, int x, int y)
{

    float sigmad = 5.0; //
    float sigmar = 0.001; //0.1% of the total range of color values
    int window = 3;
    float red0 = original(x, y).r;
    float green0 = original(x, y).g;
    float blue0 = original(x, y).b;
    float redf = flash(x, y).r;
    float greenf = flash(x, y).g;
    float bluef = flash(x, y).b;
    int i, j;
    float gd;
    float red, green, blue;
    float red2 = 0.0;
    float green2 = 0.0;
    float blue2 = 0.0;
    float gr;
    float sumweight = 0.0;
    
    
    for(i = -window; i <= window; i++)
    {
        for(j = -window; j <= window; j++)
        {
            if((x + i < original.width()) && (x + i >= 0) && (y + j < original.height()) && (y + j >= 0))
            {

                
                gd = exp(- (float)(i * i + j * j) / (2 * sigmad * sigmad));
                //std::cerr <<gd << std::endl;

                red = flash(x + i, y + j).r;
                green = flash(x + i, y + j).g;
                blue = flash(x + i, y + j).b;
            
                gr = exp(- ((redf - red) * (redf - red) + (greenf - green) * (greenf - green) + (bluef - blue) * (bluef - blue)) / (2 * sigmar * sigmar));

                sumweight += gd * gr;
		
		red2 += original(x + i, y + j).r * gd * gr;
                green2 += original(x + i, y + j).g * gd * gr;
                blue2 += original(x + i, y + j).b * gd * gr;
            }
        }
    }
    
    original(x, y).r = red2 / sumweight;
    original(x, y).g = green2 / sumweight;
    original(x, y).b = blue2 / sumweight; 
}

void imgdetail(image &base, image flash)
{
    int i, j;
    color<float> e(0.02, 0.02, 0.02); //0.02
    
    for(i = 0; i < base.width() ; i++)
    {
        for(j = 0; j < base.height(); j++)
	{
	    base(i, j) = (flash(i, j) + e) / (base(i, j) + e);
	}
    }
}

void imgmask(image &original, image flash)
{
	int i, j;
	color<float> t(0.95, 0.95, 0.95); //95% of range of sensor output values
	color<float> m1(1.0, 1.0, 1.0);
	color<float> m0(0.0, 0.0, 0.0);

	for(i = 0; i < original.width(); i++)
	{
		for(j = 0; j < original.height(); j++)
		{
			if(flash(i, j) - original(i, j) < t)
			{
				original(i, j) = m1;
			}
			else
			{
				original(i, j) = m0;
			}
		}
	}
}

void imgfinal(image &base, image nr, image detail, image mask)
{
	int i, j;
	color<float> m1(1.0, 1.0, 1.0);

	for(i = 0; i < base.width(); i++)
	{
		for(j = 0; j < base.height(); j++)
		{
			base(i, j) = (m1 - mask(i, j)) * nr(i, j) * detail(i, j) + mask(i, j) * base(i, j);				             
		}
	}
}

void whitebalance(image &original, image flash)
{
    int i, j;
    int m = 0;
    int n = 0;
    int l = 0;
    float t1 = 0.02; //2% of the range of color values
    float t2 = 0.02; //2% of the range of color values
    color<float> dp(0.0, 0.0, 0.0);
    color<float> cp(0.0, 0.0, 0.0);
    color<float> sumcp(0.0, 0.0, 0.0);
    color<float> c(0.0, 0.0, 0.0);

    for(i = 0; i < original.width(); i++)
    {
	for(j = 0; j < original.height(); j++)
	{
	    dp = flash(i, j) - original(i, j);
	    if((abs(original(i, j).r) >= t1) && (dp.r >= t2))
	    {      
		cp.r = original(i, j).r / dp.r;
		sumcp.r += cp.r;
		m++;
	    }
	    
	    if((abs(original(i, j).g) >= t1) && (dp.g >= t2))
	    {      
		cp.g = original(i, j).g / dp.g;
		sumcp.g += cp.g;
		n++;
	    }
	    
	    if((abs(original(i, j).b) >= t1) && (dp.b >= t2))
	    {      
		cp.b = original(i, j).b / dp.b;
		sumcp.b += cp.b;
		l++;
	    }
	}
    }
    
    c.r = sumcp.r / (float)m;
    c.g = sumcp.g / (float)n;
    c.b = sumcp.b / (float)l;
    for(i = 0; i < original.width(); i++)
    {
	for(j = 0; j < original.height(); j++)
	{
	    original(i, j) /= c;
	}
    }
}

int main(int argc, char** argv)
{           
  // parse commend line
  if(argc != 6)
  {
    std::cerr << "Usage: " << argv[0] << "<input original image name> <input original image name> <input original image name> <input original image name> <output image name>" << std::endl;
    return -1;
  }

    // load and save images
    image img0, img1;
    image img2, img3;
    ::io::importImage(argv[1], img0);
    ::io::importImage(argv[2], img1);
    ::io::importImage(argv[3], img2);
    ::io::importImage(argv[4], img3);

    int i, j;
    int i0, j0;
    int width, height;

    i0 = 0;
    j0 = 0;
    width = img0.width();
    height = img0.height();
    
    std::cerr <<img0.width() << std::endl;
    std::cerr <<img0.height() << std::endl;
    std::cerr <<img0.size() << std::endl;
    
    /*for(i = i0; i < width ; i++)
    {
        for(j = j0; j < height; j++)
        {
            bilateralfilter(img0, i, j);

        }
    }*/
    
    /*for(i = i0; i < width ; i++)
    {
        for(j = j0; j < height; j++)
        {
            jointbilateralfilter(img0, img1, i, j);

        }
    }*/
   
    //imgdetail(img0, img1);

    //imgmask(img0, img1);
    
    imgfinal(img0, img1, img2, img3);
    
    //whitebalance(img0, img1);
    
    ::io::exportImage(argv[3], img0);
    
    

    // Done.
    return 0;
}
