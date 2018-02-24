
#include "GroupingManager.h"
#include <time.h>
#include "Utility.h"

int main(int argv, char **argc){ 
	if (argv != 11){
		cout << "Usage: CycleGrouping.exe [in_contour_file] [in_vol_file] [in_vol_bbox_file] [out_dir] [tet_vol_limit] [random_walk_beta] [n_fair_out_loop] [n_fair_loop] [n_fair_in_loop] [genus] " << endl;
		return 1;
	}
	cout << " = INPUT ====================================="<< endl;
	cout << " - in_ctrFile = " << argc[Utility::INCONTOUR] << endl;
	cout << " - in_volFile = " << argc[Utility::INVOL] << endl;
	cout << " - in_bboxFile = " << argc[Utility::INBBOX] << endl;
	cout << " - out_dir = " << argc[Utility::OUTDIR] << endl;
	cout << " - tetlimit = " << argc[Utility::TETLIMIT] << endl;
	cout << " - beta = " << argc[Utility::RANDWBETA] << endl;
	cout << " - NBefLoop = " << argc[Utility::NBEFLOOP] << endl;
	cout << " - NLoop = " << argc[Utility::NLOOP] << endl;
	cout << " - NInLoop = " << argc[Utility::NINLOOP] << endl;
	cout << " - Genus = " << argc[Utility::FIRSTGENUS] << endl;

	GroupingManager gm;
	clock_t start, end;
	vector<float> times;

	float _teta = atof(argc[Utility::TETLIMIT]);
	float _randwb = atof(argc[Utility::RANDWBETA]);
	int _nSmBefLoop = atoi(argc[Utility::NBEFLOOP]);
	int _nLoop = atoi(argc[Utility::NLOOP]);
	int _nSmInLoop = atoi(argc[Utility::NINLOOP]);
	int _genus = atoi(argc[Utility::FIRSTGENUS]);


	cout << "\n[1] Preprocessing data & Exploring topologies in cells ..." << endl;
	start = clock();
	gm.setPara(argc[Utility::OUTDIR], _teta, _randwb, _nSmBefLoop, _nLoop, _nSmInLoop);
	gm.Contour2Arrangement(argc[Utility::INCONTOUR], argc[Utility::INVOL], argc[Utility::INBBOX]);
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
	cout << "Time: "<< times[times.size()-1] << "s \n\n\n";


	cout << "[2] Calculating search order ..." << endl;
	start = clock();
	gm.CalcSearchOrder();
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
	cout << "Time: "<< times[times.size()-1] << "s \n\n\n" << endl;


	cout << "[3] Finding global grouping ..." << endl;
	start = clock();
	gm.setGenus(_genus);
	gm.Grouping();
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
	cout << "Time: "<< times[times.size()-1] << "s \n\n\n" << endl;


	cout << "[4] Generating surfaces ... " << endl;
	start = clock();
	if(gm.solFound()){
		gm.GenSurface();
	}
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
	cout << "Time: "<< times[times.size()-1] << "s \n\n\n" << endl;
	gm.SaveResBefSmooth();


	cout << "[5] Smoothing surfaces ... " << endl;
	start = clock();
	gm.SmoothSurface();
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
	cout << "Time: "<< times[times.size()-1] << "s \n\n\n" << endl;


	// gm.writeOutputs((gm.outDir+"time.txt").c_str(), times);
	cout << "\nTime of cycle grouping: " << times[0]+times[1]+times[2]+times[3] << "s " << endl;
	cout << "\nTime of surface smoothing: " << times[4] << "s " << endl;
	cout << "\nTime in total: " << times[0]+times[1]+times[2]+times[3]+times[4] << "s " << endl;

	return 0;
}
