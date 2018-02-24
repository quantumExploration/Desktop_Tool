typedef struct visibilityProperty{
  vector<int> symmetrytype;
  svIntArray visiblelayer;
  int frequency;
  svScalar min_mag;
  svScalar max_mag;
  svVector3 lb; 
  svVector3 rb;
  svScalar min_xangle;
  svScalar max_xangle;
  svScalar min_yangle;
  svScalar max_yangle;
  svScalar min_zangle;
  svScalar max_zangle;
  int samplesize;
} visibilityProperty;
