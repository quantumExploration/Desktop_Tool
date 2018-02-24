#include <fstream>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

struct Spin
{
	double px;
	double py;
	double pz;
	
	double vx;
	double vy;
	double vz;
	
	double den;

	double region;
	double atom;
};

bool Sort_Spin_Z(const Spin &lhs, const Spin &rhs);
bool Sort_Spin_Y(const Spin &lhs, const Spin &rhs);
bool Sort_Spin_X(const Spin &lhs, const Spin &rhs);

class genVTKfromOrg
{
  public:
    genVTKfromOrg();
    virtual ~genVTKfromOrg(){cleanup();}
    
    int getNumOfIntegerDigits(double num);
    int CheckFormat(char *file);
     
    void ReadRawData(char *file, int DEN_SCALAR);
    void SplitData(); // only split by z layer; not used anymore
    void SplitData(double pos_x, double pos_y, double pos_z,
			double dir_x, double dir_y, double dir_z,double distance);

    void Compute2DData();
    void ComputeMeshPoints(char *BIN_DIR, char *dir);
    void ComputeMesh(char *srcdir,char *dir, char *outfname);
    void ComputeConvexMesh(char *srcdir, char *ifname, char *ofname, int region);

    void SetFormat(int f){format = f;}
    void SaveFormat(char *dir, char *file);  
    void SaveRegions(char *dir, char *file);

    void SaveDataByLayer(char *dir, char *rawfile, char *twodfile); 

    void SaveSortedData(char *file);
    void SaveAllData(char *file);
    void SavetoVTK(char *file);
    void SaveSplitData(char *file);
    void SaveIndex(char *file);
    void SaveRegiontoFile(char *file);
    void SaveDataFormat(char *file); 
    void SaveDensityBylayer(char *dir, char *file);
    void Save2DDensity(char *file);
    void SaveLayerFormat(char *file);
    void SaveZFormat(char *file);
    
  protected:
    void GenerateCoordinates(Spin p1, Spin p2, double *z, double *xAxis, double *yAxis);
    void SetNewCoordinates(Spin p, Spin origin, double *xAxis, double *yAxis,
                           double &x, double &y);
    void cleanup();
    
  private:
    vector<Spin> data;
    vector<int> splitReverseIndex[2];
    vector< vector<int> > splitIndex;
    vector< vector<Spin> > splitData;
    vector< vector<Spin> > split2dData;
    vector<double> xCoor[3];
    vector<double> yCoor[3];
    vector<double> coor[3];
    vector<int> Zlayer_Num;
    double lbbox[3];
    
    double rbbox[3];
    
    vector<int> shape_type;
    double min_zlayer_value;
    double min_zdistance;
    int scaling;
    int dataSize;
    double min_den;
    double min_den_zero;
    double max_den;
    double min_xdistance;
    double min_ydistance;
	vector<double> layerDensity;

    vector<int> unique_region;
    vector<double> region_z;
    vector<double> region_sphere_lb[3];
    vector<double> region_sphere_rb[3];
    vector<double> region_plane_z;
    vector<int> region_count;
    vector<int> xDim;
    vector<int> yDim;

    int format;
    char *planename;
	vector<double> plane_min;
	vector<double> plane_dir;
	double plane_distance;

    int min_region;
};
