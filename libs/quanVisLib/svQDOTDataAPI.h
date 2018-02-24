#include "svQDOTData.h"

namespace __std_lib{
 typedef struct svQDOTDataAPI{
    svQDOTDataAPI(){};
    ~svQDOTDataAPI(){};

    void setSliceData(svSliceData *sliceData);
    void setClusterData(svClusterData *clusterData);
    void set

    svSliceData *sliceData;
    svClusterData *clusterData;
    svInOutData *inOutData;
    svContourData *contourData;
    svContourClusterData *contourClusterData;
 };
}
