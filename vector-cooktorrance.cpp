#include <mitsuba/render/bsdf.h>
#include <mitsuba/hw/basicshader.h>
#include <mitsuba/core/warp.h>

MTS_NAMESPACE_BEGIN

class CookTorrance : public BSDF {
public:
	CookTorrance(const Properties &props)
		: BSDF(props) { 
	    m_diffuseReflectance = props.getSpectrum("diffuseReflectance", Spectrum(0.5f));
	    m_specularReflectance = props.getSpectrum("specularReflectance", Spectrum(0.2f));
	    m_roughness = props.getFloat("roughness", 0.1f);
	    //m_F0 = props.getFloat("F0", 0.1f);
            
            m_F0 = props.getVector("F0", Vector(0.1f));
            m_alpha = props.getVector("alpha", Vector(0.1f));
            m_p = props.getVector("p", Vector(0.1f));
            m_kappa = props.getVector("kappa", Vector(0.1f));
            
            m_lambda = props.getVector("lambda", Vector(0.1f));
            m_c = props.getVector("c", Vector(0.1f));
            m_theta0 = props.getVector("theta0", Vector(0.1f));
            m_k = props.getVector("k", Vector(0.1f));
            
            m_F1 = props.getVector("F1", Vector(0.1f));
	}

	CookTorrance(Stream *stream, InstanceManager *manager)
		: BSDF(stream, manager) {
	    m_diffuseReflectance = Spectrum(stream);
	    m_specularReflectance = Spectrum(stream);
	    m_roughness = stream->readFloat();
	    m_F0 = stream->readFloat();
            
            m_F0 = Vector(stream);
            m_alpha = Vector(stream);
            m_p = Vector(stream);
            m_kappa = Vector(stream);
            
            m_lambda = Vector(stream);
            m_c = Vector(stream);
            m_theta0 = Vector(stream);
            m_k = Vector(stream);
            
            m_F1 = Vector(stream);


	    configure();
	}

	void configure() {
		m_components.clear();
		m_components.push_back(EGlossyReflection | EFrontSide );
		m_components.push_back(EDiffuseReflection | EFrontSide );
		m_usesRayDifferentials = false;

		Float dAvg = m_diffuseReflectance.getLuminance(),
		      sAvg = m_specularReflectance.getLuminance();
		m_specularSamplingWeight = sAvg / (dAvg + sAvg);

		BSDF::configure();
	}

	Spectrum eval(const BSDFSamplingRecord &bRec, EMeasure measure) const {
	        /* sanity check */
	        if(measure != ESolidAngle || 
		   Frame::cosTheta(bRec.wi) <= 0 ||
	           Frame::cosTheta(bRec.wo) <= 0)
		  return Spectrum(0.0f);

  	        /* which components to eval */
		bool hasSpecular = (bRec.typeMask & EGlossyReflection)
				&& (bRec.component == -1 || bRec.component == 0);
		bool hasDiffuse  = (bRec.typeMask & EDiffuseReflection)
				&& (bRec.component == -1 || bRec.component == 1);

		/* eval spec */
		Spectrum result(0.0f);
		if (hasSpecular) {
		        Vector H = normalize(bRec.wo+bRec.wi);
			if(Frame::cosTheta(H) > 0.0f)
			{
			  // evaluate NDF
			  const Float roughness2 = m_roughness*m_roughness;
			  const Float cosTheta2 = Frame::cosTheta2(H);
			  const Float Hwi = dot(bRec.wi, H);
			  const Float Hwo = dot(bRec.wo, H);

			  /*const Float D = math::fastexp(-Frame::tanTheta2(H)/roughness2) / (roughness2 * cosTheta2*cosTheta2);*/
                
                const Float D = kappa * math::fastexp(- (alpha + Frame::tanTheta2(H)) / alpha) / (pow(alpha + Frame::tanTheta2(H)) / alpha, p) * cosTheta2 * cosTheta2 * INV_PI);


			  // compute shadowing and masking
			  /*const Float G = std::min(1.0f, std::min(
						   2.0f * Frame::cosTheta(H) * Frame::cosTheta(bRec.wi) / Hwi, 
						   2.0f * Frame::cosTheta(H) * Frame::cosTheta(bRec.wo) / Hwo          ));*/
                const Float G = std::min(1.0f, (float) (1.0 + m_lambda * (1.0 - math::fastexp(m_c * pow((math::safe_acos(Frame::cosTheta(bRec.wi)) - m_theta0), m_k)))));

			  // compute Fresnel
			  const Float F = fresnel(m_F0, Hwi, m_F1);

			  // evaluate the microfacet model
			  result += m_specularReflectance * INV_PI * D * G * F / Frame::cosTheta(bRec.wi);
			}
		}

		/* eval diffuse */
		if (hasDiffuse)
		  result += m_diffuseReflectance * INV_PI * Frame::cosTheta(bRec.wo);

		// Done.
		return result;
	}

	Float pdf(const BSDFSamplingRecord &bRec, EMeasure measure) const {
	        if (measure != ESolidAngle ||
			Frame::cosTheta(bRec.wi) <= 0 ||
			Frame::cosTheta(bRec.wo) <= 0 ||
			((bRec.component != -1 && bRec.component != 0) ||
			!(bRec.typeMask & EGlossyReflection)))
			return 0.0f;

		bool hasSpecular = (bRec.typeMask & EGlossyReflection)
				&& (bRec.component == -1 || bRec.component == 0);
		bool hasDiffuse  = (bRec.typeMask & EDiffuseReflection)
				&& (bRec.component == -1 || bRec.component == 1);

		Float diffuseProb = 0.0f, specProb = 0.0f;

		//* diffuse pdf */
		if (hasDiffuse)
			diffuseProb = warp::squareToCosineHemispherePdf(bRec.wo);

		/* specular pdf */
		if (hasSpecular) {
			Vector H = bRec.wo+bRec.wi;   Float Hlen = H.length();
			if(Hlen == 0.0f) specProb = 0.0f;
			else
			{
			  H /= Hlen;
			  const Float roughness2 = m_roughness*m_roughness;
			  const Float cosTheta2 = Frame::cosTheta2(H);
			  specProb = INV_PI * Frame::cosTheta(H) * math::fastexp(-Frame::tanTheta2(H)/roughness2) / (roughness2 * cosTheta2*cosTheta2) / (4.0f * absDot(bRec.wo, H));
			}
		}

		if (hasDiffuse && hasSpecular)
			return m_specularSamplingWeight * specProb + (1.0f-m_specularSamplingWeight) * diffuseProb;
		else if (hasDiffuse)
			return diffuseProb;
		else if (hasSpecular)
			return specProb;w
		else
			return 0.0f;
	}

	Spectrum sample(BSDFSamplingRecord &bRec, Float &pdf, const Point2 &_sample) const {
	        Point2 sample(_sample);


		bool hasSpecular = (bRec.typeMask & EGlossyReflection)
				&& (bRec.component == -1 || bRec.component == 0);
		bool hasDiffuse  = (bRec.typeMask & EDiffuseReflection)
				&& (bRec.component == -1 || bRec.component == 1);


		if (!hasSpecular && !hasDiffuse)
			return Spectrum(0.0f);


		// determine which component to sample
		bool choseSpecular = hasSpecular;
		if (hasDiffuse && hasSpecular) {
			if (sample.x <= m_specularSamplingWeight) {
				sample.x /= m_specularSamplingWeight;
			} else {
				sample.x = (sample.x - m_specularSamplingWeight)
					/ (1.0f-m_specularSamplingWeight);
				choseSpecular = false;
			}
		}


		/* sample specular */
		if (choseSpecular) {
			Float cosThetaM = 0.0f, phiM = (2.0f * M_PI) * sample.y;
			Float tanThetaMSqr = -m_roughness*m_roughness * math::fastlog(1.0f - sample.x);
			cosThetaM = 1.0f / std::sqrt(1.0f + tanThetaMSqr);
			const Float sinThetaM = std::sqrt(std::max((Float) 0.0f, 1.0f - cosThetaM*cosThetaM));
			Float sinPhiM, cosPhiM;
			math::sincos(phiM, &sinPhiM, &cosPhiM);

			const Normal m = Vector(sinThetaM * cosPhiM,sinThetaM * sinPhiM,cosThetaM);

			// Perfect specular reflection based on the microsurface normal
			bRec.wo = 2.0f * dot(bRec.wi, m) * Vector(m) - bRec.wi;
			bRec.sampledComponent = 0;
			bRec.sampledType = EGlossyReflection;

	        /* sample diffuse */
		} else {
	   	        bRec.wo = warp::squareToCosineHemisphere(sample);
			bRec.sampledComponent = 1;
			bRec.sampledType = EDiffuseReflection;
		}
		bRec.eta = 1.0f;

		pdf = CookTorrance::pdf(bRec, ESolidAngle);

		/* unoptimized evaluation, explicit division of evaluation / pdf. */
		if (pdf == 0 || Frame::cosTheta(bRec.wo) <= 0)
			return Spectrum(0.0f);
		else
			return eval(bRec, ESolidAngle) / pdf;
	}

	Spectrum sample(BSDFSamplingRecord &bRec, const Point2 &sample) const {
    	        Float pdf;
		return CookTorrance::sample(bRec, pdf, sample);
	}

	void serialize(Stream *stream, InstanceManager *manager) const {
		BSDF::serialize(stream, manager);

		m_diffuseReflectance.serialize(stream);
		m_specularReflectance.serialize(stream);
		stream->writeFloat( m_roughness );
        //stream->writeFloat( m_F0 );
        
		m_F0.serialize(stream);
        m_alpha.serialize(stream);
        m_p.serialize(stream);
        m_kappa.serialize(stream);
        
        m_lambda.serialize(stream);
        m_c.serialize(stream);
        m_theta0.serialize(stream);
        m_k.serialize(stream);
        
        m_F1.serialize(stream);
	}

	Float getRoughness(const Intersection &its, int component) const {
	       return m_roughness;
	}

	std::string toString() const {
	       std::ostringstream oss;
 	       oss << "Cook-Torrance[" << endl
	           << " id = \"" << getID() << "\"," << endl
		   << " diffuseReflectance = " << indent(m_diffuseReflectance.toString()) << ", " << endl
		   << " specularReflectance = " << indent(m_specularReflectance.toString()) << ", " << endl
           //<< " F0 = " << m_F0 << ", " << endl
           << " roughness = " << m_roughness << endl
        
        << " F0 = " << indent(m_F0.toString()) << ", " << endl
        << " alpha = " << indent(m_alpha.toString()) << ", " << endl
        << " p = " << indent(m_p.toString()) << ", " << endl
        << " kappa = " << indent(m_kappa.toString()) << ", " << endl
        
        << " lambda = " << indent(m_lambda.toString()) << ", " << endl
        << " c = " << indent(m_c.toString()) << ", " << endl
        << " theta0 = " << indent(m_theta0.toString()) << ", " << endl
        << " k = " << indent(m_k.toString()) << ", " << endl
        
        << " F1 = " << indent(m_F1.toString()) << ", " << endl
        
		   << "]";
	       return oss.str();
	}

	Shader *createShader(Renderer *renderer) const;

	MTS_DECLARE_CLASS()
private:
	// helper method
	inline Float fresnel(const Float& F0, const Float& c, const Float& F1) const
        {
	  /*return F0 + (1.0f - F0)*pow(1.0-c, 5.0f);*/
            return F0 + (1.0f - F0)*pow(1.0-c, 5.0f) - F1 * c;
	}

	// attribtues
        //Float m_F0;
        Float m_roughness;
        Spectrum m_diffuseReflectance;
        Spectrum m_specularReflectance;
    Vector m_F0;
    Vector m_alpha;
    Vector m_p;
    Vector m_kappa;
    Vector m_lambda;
    Vector m_c;
    Vector m_theta0;
    Vector m_k;
    Vector m_F1;

        Float m_specularSamplingWeight;
};

// ================ Hardware shader implementation ================

/* CookTorrance shader-- render as a 'black box' */
class CookTorranceShader : public Shader {
public:
	CookTorranceShader(Renderer *renderer) :
		Shader(renderer, EBSDFShader) {
		m_flags = ETransparent;
	}

	void generateCode(std::ostringstream &oss,
			const std::string &evalName,
			const std::vector<std::string> &depNames) const {
		oss << "vec3 " << evalName << "(vec2 uv, vec3 wi, vec3 wo) {" << endl
			<< "    return vec3(0.0);" << endl
			<< "}" << endl;
		oss << "vec3 " << evalName << "_diffuse(vec2 uv, vec3 wi, vec3 wo) {" << endl
			<< "    return vec3(0.0);" << endl
			<< "}" << endl;
	}
	MTS_DECLARE_CLASS()
};

Shader *CookTorrance::createShader(Renderer *renderer) const {
	return new CookTorranceShader(renderer);
}

MTS_IMPLEMENT_CLASS(CookTorranceShader, false, Shader)
MTS_IMPLEMENT_CLASS_S(CookTorrance, false, BSDF)
MTS_EXPORT_PLUGIN(CookTorrance, "CookTorrance BSDF");
MTS_NAMESPACE_END
