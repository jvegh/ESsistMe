#include "MnUserParameters.h"

MnUserParameters::MnUserParameters(const std::vector<double>& par, const std::vector<double>& err) : theTransformation(par, err) {}

const std::vector<MinuitParameter>& MnUserParameters::parameters() const {
  return theTransformation.parameters();
}

std::vector<double> MnUserParameters::params() const {
  return theTransformation.params();
}

std::vector<double> MnUserParameters::errors() const {
  return theTransformation.errors();
}

const MinuitParameter& MnUserParameters::parameter(unsigned int n) const {
  return theTransformation.parameter(n);
}

void MnUserParameters::add(const char* name, double val, double err) {
  theTransformation.add(name, val, err);
}

void MnUserParameters::add(const char* name, double val, double err, double low, double up) {
  theTransformation.add(name, val, err, low, up);
}

void MnUserParameters::add(const char* name, double val) {
  theTransformation.add(name, val);
}

#if MINUIT_EXTENDED
/*! add a MinuitParameter, with a name combined from the set and parameter name */
void MnUserParameters::add(MinuitParameter &MP, char * setName) {
	char MyName[11];
	int l = std::min(int(strlen(setName)), 11);
    memset(MyName, 0, 11*sizeof(char));
    memcpy(MyName, setName, l*sizeof(char));
	if(l>7)
	{	//memcpy(&theName[0], &theName[l-7], 7); 
    MyName[7] = '\0'; }
    // Now the pm set name is OK
    char theName[11]; 
	memset(theName, 0, 11*sizeof(char)); 
	l = std::min(int(strlen(MP.GetLabel())), 11);
	memcpy(theName, MP.GetLabel(), l*sizeof(char));
	if(strlen(MP.GetLabel())>3)
		theName[3] = '\0';
    strcat(MyName, theName);    // The combined label is 7 char set label + 3 char param label
	theTransformation.add(MP, MyName);
}

bool MnUserParameters::extract(MinuitParameter &MP) {
	int ui = MP.GetUserIndex();
    if(-1 == ui)
        return false; // This pm is not put in the array
    MP.setValue(parameters()[ui].value());
    MP.setError(parameters()[ui].error());
    return true;
}

/// add set of Minuit parameters
void MnUserParameters::add(MinuitParameterSet &MPS) {
    for( unsigned int i = 0; i<MPS.GetCount(); i++)
    {
        char L[11]; memset(L, 0, 11*sizeof(char)); strcpy(L,MPS.GetLabel());
        add(MPS[i], L);
    }
}

/// extract value and error from Minuit parameters
bool MnUserParameters::extract(MinuitParameterSet &MPS) {
    for( unsigned int i = 0; i<MPS.GetCount(); i++)
        if (!extract(MPS[i])) return false;
    return true;
}
#endif

void MnUserParameters::fix(unsigned int n) {
  theTransformation.fix(n);
}

void MnUserParameters::release(unsigned int n) {
  theTransformation.release(n);
}

void MnUserParameters::setValue(unsigned int n, double val) {
  theTransformation.setValue(n, val);
}

void MnUserParameters::setError(unsigned int n, double err) {
  theTransformation.setError(n, err);
}

void MnUserParameters::setLimits(unsigned int n, double low, double up) {
  theTransformation.setLimits(n, low, up);
}

void MnUserParameters::setUpperLimit(unsigned int n, double up) {
  theTransformation.setUpperLimit(n, up);
}

void MnUserParameters::setLowerLimit(unsigned int n, double low) {
  theTransformation.setLowerLimit(n, low);
}

void MnUserParameters::removeLimits(unsigned int n) {
  theTransformation.removeLimits(n);
}

double MnUserParameters::value(unsigned int n) const {
  return theTransformation.value(n);
}

double MnUserParameters::error(unsigned int n) const {
  return theTransformation.error(n);
}

void MnUserParameters::fix(const char* name) {
  fix(index(name));
}

void MnUserParameters::release(const char* name) {
  release(index(name));
}

void MnUserParameters::setValue(const char* name, double val) {
  setValue(index(name), val);
}

void MnUserParameters::setError(const char* name, double err) {
  setError(index(name), err);
}

void MnUserParameters::setLimits(const char* name, double low, double up) {
  setLimits(index(name), low, up);
}

void MnUserParameters::setUpperLimit(const char* name, double up) {
  theTransformation.setUpperLimit(index(name), up);
}

void MnUserParameters::setLowerLimit(const char* name, double low) {
  theTransformation.setLowerLimit(index(name), low);
}

void MnUserParameters::removeLimits(const char* name) {
  removeLimits(index(name));
}

double MnUserParameters::value(const char* name) const {
  return value(index(name));
}

double MnUserParameters::error(const char* name) const {
  return error(index(name));
}
  
unsigned int MnUserParameters::index(const char* name) const {
  return theTransformation.index(name);
}

const char* MnUserParameters::name(unsigned int n) const {
  return theTransformation.name(n);
}

const MnMachinePrecision& MnUserParameters::precision() const {
  return theTransformation.precision();
}
