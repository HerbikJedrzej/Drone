#include "TrybeTools.hh"

double roundRadioSetValueToDouble(const int8_t& val, const double& factor){
	if(val > 4){
		return (val - 4) / factor;
	}
	if(val < -5){
		return (val + 5) / factor;
	}
	return 0.0;
}
