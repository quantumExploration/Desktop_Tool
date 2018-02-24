#include "Arrangement.h"

void Arrangement::constructArrangement(
	int ssvernum, int ssedgenum, int ssfacenum, int ssspacenum,
	float *ssver, int *ssedge, int **ssface, int **ssspace,
	int *ssfaceedgenum, int *ssspacefacenum, int **ssspace_planeside, int *ssface_planeindex, 

	int planenum, float *pparam, float cellFrame [], 

	int *ctrfvernum, int *ctrfedgenum,
	float **ctrfverpos, int **ctrfvertype, int **ctrfverval,
	int **ctrfedge, int **ctrfedgetype, int **ctrfedgeval
	){
		/* -------------------------------------------------------------------- */
		/* ------------------- MING: construct Arrangement -------------------- */
		/* Section 1: Construct Vs, seg2V, F2seg */
		vector<vector<float> > verOnCellE;
		vector<vector<int> > verIdOnCellE;
		vector<float> oneV(3, 0);
		vector<int> oneE(2, 0);
		vector<int> oneSeg;
		vector<int> segEnds;
		vector<int> ind2ind;
		vector<vector<int> > nbVs; // will be empty after tracing the segs, do not plan to use it afterwards
		vector<vector<int> > nbVsLeftMaterial; // will be empty after tracing the segs, do not plan to use it afterwards
		set<int> edgeMats;
		int matchVi, verOnCellEi, v0, v1;
		verOnCellE.resize(ssedgenum);
		verIdOnCellE.resize(ssedgenum);
		F2seg.resize(ssfacenum);
		nF = ssfacenum;
		nSegV = 0;

		vector<int> toDelSegVs;

		for(int i=0; i<3; ++i){
			cellFrameLowerCorner[i] = cellFrame[i];
			cellFrameScale[i] = cellFrame[3+i]-cellFrame[i];
		}
		for (int fi = 0; fi < ssfacenum; ++fi){
			if (ctrfvernum[fi] == 0){
				continue;
			}
			segEnds.clear();
			ind2ind.clear();
			ind2ind.resize(ctrfvernum[fi]);
			for (int vi = 0; vi < ctrfvernum[fi]; ++vi){

				/*Step 1: store all V in current face into arrangement Vs*/
				oneV[0] = ctrfverpos[fi][3 * vi + 0];
				oneV[1] = ctrfverpos[fi][3 * vi + 1];
				oneV[2] = ctrfverpos[fi][3 * vi + 2];
				// ver is inside of the face completely
				if (ctrfvertype[fi][vi] == /*CTRTYPE_SUBFACE*/3){
					ind2ind[vi] = (int)Vs.size();
					Vs.push_back(oneV);
					V2seggV.push_back(-1);
				}
				// ver is on some cell edge
				else {
					segEnds.push_back(vi);
					verOnCellEi = ctrfverval[fi][vi];
					matchVi = findMatchVer(verOnCellE[verOnCellEi], oneV);
					// a new intersection ver is found
					if (matchVi == -1){
						ind2ind[vi] = (int)Vs.size();
						Vs.push_back(oneV);
						verIdOnCellE[verOnCellEi].push_back(ind2ind[vi]);
						verOnCellE[verOnCellEi].push_back(oneV[0]);
						verOnCellE[verOnCellEi].push_back(oneV[1]);
						verOnCellE[verOnCellEi].push_back(oneV[2]);
						V2seggV.push_back(nSegV++);
					}
					// the intersection ver is processed before
					else{
						ind2ind[vi] = verIdOnCellE[verOnCellEi][matchVi];
					}
				}
			}
			/*Step 2: trace segment out from stored edges*/

			int leftMaterial;
			nbVs.clear();
			nbVsLeftMaterial.clear();
			nbVs.resize(ctrfvernum[fi]);
			nbVsLeftMaterial.resize(ctrfvernum[fi]);
			// go through all edges, store the neighborhood
			for (int ei = 0; ei < ctrfedgenum[fi]; ++ei){
				v0 = ctrfedge[fi][4 * ei + 0];
				v1 = ctrfedge[fi][4 * ei + 1];
				nbVs[v0].push_back(v1);
				nbVs[v1].push_back(v0);
				nbVsLeftMaterial[v0].push_back(ctrfedge[fi][4 * ei + 2]);
				nbVsLeftMaterial[v1].push_back(ctrfedge[fi][4 * ei + 3]);
				edgeMats.insert(ctrfedge[fi][4 * ei + 2]);
				edgeMats.insert(ctrfedge[fi][4 * ei + 3]);
			}
			// for now, we only consider 2 materials: in/out
			matMarks = vector<int>(edgeMats.begin(), edgeMats.end());

			// assumption here: segments in each face are disjoint completely
			// so 2 nbs for interior ver; 1 for end ver;
			// handle open segment first
			vector<bool> unvisited(ctrfvernum[fi], true);
			for (int end : segEnds){
				if (!nbVs[end].empty()){
					// a new open segment is found
					traceOneSeg(nbVs, nbVsLeftMaterial, ind2ind, unvisited, oneSeg, end, leftMaterial);

					// handle the bug where the newly added intersection points are too close to its nb, so tetgen will crash later
					// remove the close nbV(s)
					cleanTwoEndsOfOneSeg(oneSeg, toDelSegVs);

					F2seg[fi].push_back((int)seg2V.size());
					seg2F.push_back(fi);

					seg2V.push_back(oneSeg);
					seg2lmat.push_back(leftMaterial);
				}
			}
			// handle closed segment
			int end;
			for (int vi = 0; vi < ctrfvernum[fi]; ++vi){
				// a new closed segment is found
				if (unvisited[vi]){
					end = vi;
					traceOneSeg(nbVs, nbVsLeftMaterial, ind2ind, unvisited, oneSeg, end, leftMaterial);
					F2seg[fi].push_back((int)seg2V.size());
					seg2F.push_back(fi);
					seg2V.push_back(oneSeg);
					seg2lmat.push_back(leftMaterial);
				}
			}

		}
		nSeg = (int)seg2V.size();
		for(size_t i=0; i<nSeg; ++i){
			dealDegeneratedOneSegWithTwoV(seg2V[i]);
		}

		if(!toDelSegVs.empty()){
			sort(toDelSegVs.begin(), toDelSegVs.end());
			// seg2V
			for(size_t i=0; i<seg2V.size(); ++i){
				for(size_t j=0; j<seg2V[i].size(); ++j){
					seg2V[i][j] = seg2V[i][j] - Utility::findNEleSmallerInSortedList(toDelSegVs, seg2V[i][j]);
				}
			}
			// verIdOnCellE
			for(size_t i=0; i<verIdOnCellE.size(); ++i){
				for(size_t j=0; j<verIdOnCellE[i].size(); ++j){
					verIdOnCellE[i][j] = verIdOnCellE[i][j] - Utility::findNEleSmallerInSortedList(toDelSegVs, verIdOnCellE[i][j]);
				}
			}
			// V2seggv
			vector<int> new_V2seggV(Vs.size()-toDelSegVs.size(),-1);
			for(int vi=0; vi<V2seggV.size(); ++vi){
				if(V2seggV[vi]!=-1){
					new_V2seggV[vi - Utility::findNEleSmallerInSortedList(toDelSegVs, vi)] = V2seggV[vi];
				}
			}
			V2seggV = new_V2seggV;
			// Vs
			for(int i=toDelSegVs.size()-1; i>=0; i--){
				Vs.erase(Vs.begin()+toDelSegVs[i]);
			}
		}

		//-----------------------------------------------------------------------------------------------------------------------------------------

		/* Section 2: Construct segGraph */
		segGraph.resize(nSegV);
		segGraph_b.resize(nSegV, BigInt(nSeg));

		for (int si = 0; si < nSeg; ++si){
			v0 = V2seggV[seg2V[si].front()];
			v1 = V2seggV[seg2V[si].back()];
			if (v0 != v1){
				segGraph[v0].push_back(si);
				segGraph[v1].push_back(si);
				segGraph_b[v0].set(si);
				segGraph_b[v1].set(si);
			}
		}

		/* Section 3: Construct cell2F, cellEmpty, cellGraph*/
		nCell = ssspacenum;
		nFront = nCell;
		nSeg = (int)seg2V.size();
		cell2F.resize(nCell);
		cell2Fside.resize(nCell);
		cellEmpty.resize(nCell);
		cellGraph.resize(nCell);
		cell2seg.resize(nCell);
		cell2seg_b.resize(ssspacenum, BigInt(nSeg));
		vector<vector<int> > cellsOfF;
		cellsOfF.resize(ssfacenum);
		bool isempty;
		for (int ci = 0; ci < nCell; ++ci){
			isempty = true;
			for (int fi = 0; fi < ssspacefacenum[ci]; ++fi){
				cell2F[ci].push_back(ssspace[ci][fi]);
				cell2Fside[ci].push_back(ssspace_planeside[ci][fi]);
				cellsOfF[ssspace[ci][fi]].push_back(ci);
				if (!F2seg[ssspace[ci][fi]].empty()){
					isempty = false;
				}
			}
			cellEmpty[ci] = isempty;
		}
		int c1, c2;
		for (int fi = 0; fi < ssfacenum; ++fi){
			F2PlaneID.push_back(ssface_planeindex[fi]);
			if (cellsOfF[fi].size() != 2) continue;
			c1 = cellsOfF[fi][0];
			c2 = cellsOfF[fi][1];
			cellGraph[c1].push_back(c2);
			cellGraph[c2].push_back(c1);

			for (int si : F2seg[fi]){
				cell2seg_b[c1].set(si);
				cell2seg_b[c2].set(si);
				cell2seg[c1].push_back(si);
				cell2seg[c2].push_back(si);
			}
		}

		vector<float> para;
		para.resize(4);
		for (int pi=0; pi<planenum; ++pi){
			para[0] = pparam[pi * 4];
			para[1] = pparam[pi * 4 + 1];
			para[2] = pparam[pi * 4 + 2];
			para[3] = pparam[pi * 4 + 3];
			planeParas.push_back(para);
		}

		/* Section 4: Construct cellVs and cellEs, for debug*/
		for (int vi = 0; vi < ssvernum; ++vi){
			oneV[0] = ssver[3 * vi + 0];
			oneV[1] = ssver[3 * vi + 1];
			oneV[2] = ssver[3 * vi + 2];
			cellVs.push_back(oneV);
		}
		list<pair<float, int> > sorted_VnDistOnCellE; 
		sorted_verIdOnCellE.resize(ssedgenum);
		for (int ei = 0; ei < ssedgenum; ++ei){
			oneE[0] = ssedge[2 * ei + 0];
			oneE[1] = ssedge[2 * ei + 1];
			cellEs.push_back(oneE);

			sorted_VnDistOnCellE.clear();
			for (int vi : verIdOnCellE[ei]){
				sorted_VnDistOnCellE.push_back(make_pair(EuclideanDistSquare1Cell2Seg(oneE[0], vi), vi));
			}
			sorted_VnDistOnCellE.sort();
			sorted_verIdOnCellE[ei].reserve(sorted_VnDistOnCellE.size());
			for (auto it = sorted_VnDistOnCellE.begin(); it != sorted_VnDistOnCellE.end(); ++it){
				sorted_verIdOnCellE[ei].push_back(it->second);
			}
		}

		cellFs.resize(ssfacenum);
		for (int fi = 0; fi < ssfacenum; ++fi){
			for (int ei = 0; ei < ssfaceedgenum[fi]; ++ei){
				cellFs[fi].push_back(ssface[fi][ei]);
			}
		}
		reOrderCellEinCellF();

		findCloseCyclesInCells();

		/* -------------------------------------------------------------------- */
#if _SAVEDATA_MING
		writeArrangement((t_savedir+"arrangement_beforeCell.txt").c_str());
#endif
		moveSegPtsBackToPlane();
		moveCellPtsBackToPlane();
#if _SAVEDATA_MING
		writeArrangement((t_savedir+"arrangement_beforeCell2.txt").c_str());
#endif

#if _SAVERES_MING
		writeLuCtrGraph((t_savedir+"forLu.ctrGraph").c_str());
#endif

		/*  Section 5: explor topology within each cell, tetgen + random walk + critical point*/ 

		tetV.resize(nCell);
		tetVh.resize(nCell);
		tetVmarker.resize(nCell);
		tetnV.resize(nCell);
		tetQ.resize(nCell);
		allGroupings.resize(nCell);
		allGroupingScore.resize(nCell);
		allGroupingIso.resize(nCell);
		cell2Loc_setIso.resize(nCell);
		cell2Loc_Global_setMap.resize(nCell);
		cell2Loc_setActEdge.resize(nCell);
		bitIntHash allSets_b;
		maxNCyc = -1;
		for (int ci = 0; ci < nCell; ++ci){
			if (cell2ncyc[ci] > maxNCyc){
				maxNCyc = cell2cyc[ci].size();
			}
		}
		nSets = 0;
		allCellTets.init(nCell);
#if _DO_PARALLEL_
		#pragma omp parallel for default(shared)// num_threads(8)
#endif

#if _CG_LOG
		clock_t start, end;
		start = clock();
#endif
		for (int ci = 0; ci < nCell; ++ci){
			if (cellEmpty[ci] ) continue; 
#if _CG_LOG

			cout << "   cell ["<< ci << "] : BEGIN ..." << endl;
#endif
			explorTopologyInCell(ci, allSets_b);
#if _CG_LOG
			cout << "   cell ["<< ci << "] : DONE!" << endl << "   -----------------------------------" << endl;
			end = clock();
			cout << "      Time Used Until Now: "<< (end - start) / CLOCKS_PER_SEC << "s " << endl << "   -----------------------------------" << endl;
#endif
		}
		nSets = allSets_b.size();
		allSets.resize(nSets);
		for (auto set = allSets_b.begin(); set!=allSets_b.end(); ++set){
			allSets[set->second]=(set->first.getOnesInd());
		}
		int nTets = 0;
		for(int i=0; i<nCell; ++i){
			nTets += allCellTets.cells[i].cellTets.size();
		}
		cout << "nTets = " << nTets << endl;
#if _SAVEDATA_MING
		Utility::writeVector(allSets, (t_savedir+"allSets.txt").c_str());
#endif
}


void Arrangement::moveIntersectionPtsBackToPlane(){
	set<int> intsVs;
	unordered_map<int, set<int> > intsV2PlaneIDs;
	vector<int> vFrontBack(2);
	set<int> tmpPlaneIDs;
	for(size_t i=0; i<seg2V.size(); ++i){
		vFrontBack[0] = seg2V[i].front();
		vFrontBack[1] = seg2V[i].back();
		if(vFrontBack[0]!=vFrontBack[1]){
			for(int j=0; j<2; ++j){
				// a new intersection V is found
				if(intsVs.find(vFrontBack[j])==intsVs.end()){
					intsVs.insert(vFrontBack[j]);
				}
				tmpPlaneIDs = intsV2PlaneIDs[vFrontBack[j]];
				tmpPlaneIDs.insert(F2PlaneID[seg2F[i]]);
				intsV2PlaneIDs[vFrontBack[j]] = tmpPlaneIDs;
			}
		}
	}
	for(auto vi : intsVs){
		Eigen::Vector3f curV(Vs[vi][0],Vs[vi][1],Vs[vi][2]);
		tmpPlaneIDs = intsV2PlaneIDs[vi];
		for(auto pi : tmpPlaneIDs){
			Eigen::Vector3f n(planeParas[pi][0],planeParas[pi][1],planeParas[pi][2]);
			float d = -planeParas[pi][3] / n.norm();
			n.normalize();
			curV = curV - (curV.dot(n)+d)*n;
		}
		Vs[vi][0] = curV[0];
		Vs[vi][1] = curV[1];
		Vs[vi][2] = curV[2];
	}
}


void Arrangement::writeLuCtrGraph(const char* filename){
	vector<vector<int> > plane2segs(planeParas.size());
	for(size_t segi=0; segi<seg2V.size(); ++segi){
		int pi = F2PlaneID[seg2F[segi]];
		if (pi < 0) continue;
		plane2segs[pi].push_back(segi);
	}
	ofstream ofs(filename, ofstream::out);
	if (!ofs.good()) {
		cout << "Can not write ctrGraph file " << filename << endl;
		return;
	}
	// Vs
	vector<vector<float> > scaledVs = Vs;
	Utility::scaleDnVers(scaledVs,unifyScaleRatio, unifyTransVec);
	ofs << "n " << scaledVs.size() << "\n";
	for( auto & oneV : scaledVs){
		ofs << "v " << oneV[0] << " " << oneV[1] << " " << oneV[2] << endl;
	}
	// Planes
	ofs << "n " << plane2segs.size() << "\n";
	for(size_t pi=0; pi<plane2segs.size(); ++pi){
		int nSegEdge = 0;
		for(auto segi : plane2segs[pi]){
			nSegEdge += (seg2V[segi].size()-1);
		}
		ofs << "p " << planeParas[pi][0] << " " << planeParas[pi][1] << " " << planeParas[pi][2] << " " << planeParas[pi][3] << " " << nSegEdge << endl;
		for(auto segi : plane2segs[pi]){
			int leftMat = seg2lmat[segi];
			int rightMat = TagMapping::getOtherMat(leftMat, matMarks);
			for(size_t vi=0; vi<seg2V[segi].size()-1; ++vi){
				ofs << "e " << seg2V[segi][vi] << " " << seg2V[segi][vi+1] << " " << leftMat << " " << rightMat << endl;
			}
		}
	}
	ofs.close();
}

void Arrangement::moveSegPtsBackToPlane(){
	for(size_t segi=0; segi<seg2V.size(); ++segi){
		for(auto vi : seg2V[segi]){
			Eigen::Vector3f curV(Vs[vi][0],Vs[vi][1],Vs[vi][2]);
			int pi = F2PlaneID[seg2F[segi]];
			Eigen::Vector3f n(planeParas[pi][0],planeParas[pi][1],planeParas[pi][2]);
			float d = -planeParas[pi][3] / n.norm();
			n.normalize();
			curV = curV - (curV.dot(n)+d)*n;
			Vs[vi][0] = curV[0];
			Vs[vi][1] = curV[1];
			Vs[vi][2] = curV[2];
		}	
	}
}


void Arrangement::moveCellPtsBackToPlane(){
	vector<set<int> > CellV2PlaneIDs(cellVs.size());
	int pInd;
	for(int ci=0; ci<cell2F.size(); ++ci){
		for(auto fInd : cell2F[ci]){
			if(fInd < 0) continue;

			pInd = F2PlaneID[fInd];
			for(auto eInd : cellFs[fInd]){
				CellV2PlaneIDs[cellEs[eInd][0]].insert(pInd);
				CellV2PlaneIDs[cellEs[eInd][1]].insert(pInd);
			}
		}
	}
	for(int vi=0; vi<cellVs.size(); ++vi){
		Eigen::Vector3f curV(cellVs[vi][0],cellVs[vi][1],cellVs[vi][2]);
		for(auto pi : CellV2PlaneIDs[vi]){
			if(pi < 0 ) continue;
			Eigen::Vector3f n(planeParas[pi][0],planeParas[pi][1],planeParas[pi][2]);
			float d = -planeParas[pi][3] / n.norm();
			n.normalize();
			curV = curV - (curV.dot(n)+d)*n;
			cellVs[vi][0] = curV[0];
			cellVs[vi][1] = curV[1];
			cellVs[vi][2] = curV[2];
		}
	}
}
