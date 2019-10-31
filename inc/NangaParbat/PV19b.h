//
// Authors: Valerio Bertone: valerio.bertone@cern.ch
//          Alessandro Bacchetta: alessandro.bacchetta@unipv.it

#pragma once

#include "NangaParbat/parameterisation.h"

#include <math.h>

namespace NangaParbat
{
  /**
   * @brief Pavia 2019 parameterisation derived from the
   * "Parameterisation" mother class.
   */
  class PV19b: public NangaParbat::Parameterisation
  {
  public:

    PV19b(): Parameterisation{"PV19b", 2, std::vector<double> {0, 0, 0, 0, 0, 0, 0, 0}} {};

    double Evaluate(double const& x, double const& b, double const& zeta, int const& ifunc) const
    {
      if (ifunc < 0 || ifunc >= this->_nfuncs)
        throw std::runtime_error("[PV19b::Evaluate]: function index out of range");

      // If the value of 'x' exceeds one returns zero
      if (x >= 1)
        return 0;

      // Free paraMeters
      const double g2     = this->_pars[0];
      const double N1     = this->_pars[1];
      const double alpha  = this->_pars[2];
      const double lambda = this->_pars[3];
      const double N1B    = this->_pars[4];
      const double alphaB = this->_pars[5];
      const double sigmaB = this->_pars[6];
      const double g2B    = this->_pars[7];

      // x-dependent bits
      const double g1  = exp( N1 * ( x / alpha - 1 ) );
      const double g1B = N1B * exp( - pow( ( x - alphaB ) / sigmaB, 2 ) / 2 );

      // bT-dependent bits
      const double b2 = b * b;

      // zeta-dependent bit (i.e. non perturbative evolution)
      const double lnz    = log(zeta / _Q02);
      const double NPevol = exp( - ( g2 + g2B * b2 ) * b2 * lnz / 4 );

      const double gauss  = exp( - g1B * b2 / 4 );
      const double qgauss = 1 / ( 1 + g1 * b2 / 4 );

      return ( ( 1 - lambda ) * qgauss + lambda * gauss ) * NPevol;
    };

    std::string LatexFormula() const
    {
      std::string formula;
      formula  = R"delimiter($$f_{\rm NP}(x,\zeta, b_T)= \Biggl(
\frac{1-\lambda_B^2}{1 + g_1^2(x) b_T^2/4} + \lambda_B^2 \exp \left(-g_{1B} b_T^2 /4 \right)\Biggr) \exp\left[- g_2 \log\left(\frac{\zeta}{Q_0^2}\right) b_T^2/4 - g_{2B} \log\left(\frac{\zeta}{Q_0^2}\right) b_T^4/4 \right]$$)delimiter";
      formula += R"delimiter($$g_1(x) = N_1 \frac{x^{\sigma}(1-x)^{\alpha}}{\hat{x}^{\sigma}(1-\hat{x})^{\alpha}}$$)delimiter";
      formula += R"delimiter($$g_{1B}(x) = N_{1B} \frac{x^{\sigma_B}(1-x)^{\alpha_B}}{\hat{x}^{\sigma_B}(1-\hat{x})^{\alpha_B}}$$)delimiter";
      formula += R"delimiter($$Q_0^2 = 1\;{\rm GeV}^2$$)delimiter";
      formula += R"delimiter($$\hat{x} = 0.1$$)delimiter";
      return formula;
    };

    std::vector<std::string> GetParameterNames() const
    {
      return {R"delimiter($g_2$)delimiter",
              R"delimiter($N_1$)delimiter",
              R"delimiter($\alpha$)delimiter",
              R"delimiter($\lambda_B$)delimiter",
              R"delimiter($N_{1B}$)delimiter",
              R"delimiter($\alpha_B$)delimiter",
              R"delimiter($\sigma_B$)delimiter",
              R"delimiter($g_{2B}$)delimiter"};
    };

    std::string GetDescription() const
    {
      return "Parameterisation used for the Pavia 2019 TMD analysis.";
    };

  private:
    const double _Q02 = 1;
  };
}