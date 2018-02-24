CC := g++ -Wall -g -std=c++0x
CFLAGS := -Wall -g -std=c++0x

TARGET=CycleGrouping

SRCS := main.cpp\
	Arrangement.cpp \
	Arrangement_main.cpp \
	Ctr2ArHandler.cpp \
	Arrangement_surface.cpp \
	CellTet.cpp \
	Arrangement_topology.cpp \
	global.cpp \
	mymath.cpp \
	SpacePartitioner_CommonLine.cpp \
	SpacePartitioner_db.cpp \
	SpacePartitioner.cpp \
	SpacePartitioner_CtrGraph.cpp \
	ContourHandler_CtrGraph.cpp \
	ContourHandler_db.cpp \
	ContourHandler_PartitionToFace_CommonEdge.cpp \
	ContourHandler_PartitionToFace.cpp \
	ContourHandler_PreProc_1RadialSet_Random.cpp \
	ContourHandler_PreProcCommonEdge.cpp \
	ContourHandler_PreProc.cpp \
	ContourHandler_Reader.cpp \
	meshFair.cpp \
	sort.cpp \
	VerEdgePlaneOp.cpp \
	SetOperation.cpp \
	GroupingManager.cpp \
	GroupingManager_main.cpp \
	DMWT.cpp \
	Boundary.cpp \
	Hole.cpp \
	TriangleInfo.cpp \
	EdgeInfo.cpp \
	Tile.cpp \
	CurveInfo.cpp \
	Point3.cpp \
	Vector3.cpp \
	AlgoX.cpp \
	BigInt.cpp \
	Graph.cpp \
	UnionFind.cpp  
OBJS =  Arrangement.o \
        Arrangement_main.o \
        Ctr2ArHandler.o \
        Arrangement_surface.o \
        CellTet.o \
        Arrangement_topology.o \
        global.o \
        mymath.o \
        SpacePartitioner_CommonLine.o \
        SpacePartitioner_db.o \
        SpacePartitioner.o \
        SpacePartitioner_CtrGraph.o \
        ContourHandler_CtrGraph.o \
        ContourHandler_db.o \
        ContourHandler_PartitionToFace_CommonEdge.o \
        ContourHandler_PartitionToFace.o \
        ContourHandler_PreProc_1RadialSet_Random.o \
        ContourHandler_PreProcCommonEdge.o \
        ContourHandler_PreProc.o \
        ContourHandler_Reader.o \
        meshFair.o \
        sort.o \
        VerEdgePlaneOp.o \
        SetOperation.o \
        GroupingManager.o \
        GroupingManager_main.o \
        DMWT.o \
        Boundary.o \
        Hole.o \
        TriangleInfo.o \
        EdgeInfo.o \
        Tile.o \
        CurveInfo.o \
        Point3.o \
        Vector3.o \
        AlgoX.o \
        BigInt.o \
        Graph.o \
        UnionFind.o \
	main.o

INCLUDE = -I/usr/include/boost/ -I/home/henan/Documents/Topology/CycleGrouping_v1.0.0/src/ExternalLibs/Tetgen/tetgen1.4.3/project_exe 
LIBS = -L/home/henan/Documents/Topology/CycleGrouping_v1.0.0/src/ExternalLibs/Tetgen/tetgen1.4.3/project_exe -ltet

all: $(TARGET)

%.o: %.cpp  
	$(CC) $(CFLAGS) -c $< ${INCLUDE} 

$(TARGET): $(OBJS) 
	$(CC) -o $@ $^ $(LIBS) ${INCLUDE}

clean:
	rm -rf $(TARGET) *.o
.PHONY: all clean 
