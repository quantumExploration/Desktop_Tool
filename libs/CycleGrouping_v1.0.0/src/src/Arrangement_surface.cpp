#include "Arrangement.h"


void Arrangement::sufCell_genSurface(vector<int> &resGrouping){

	clock_t start, end;
	vector<float> times;
#if _CG_LOG
	cout << "   (1) gen coarse surface ... " << endl;
#endif
	start = clock();

	allCellSufs.init(nCell);

	vector<vector<int> > seg2cells(nSeg);
	vector<vector<int> > segInCellPos(nSeg);

#if _DO_PARALLEL_
	#pragma omp parallel for default(shared)// num_threads(8)
#endif
	for (int i=0; i<nonEmpty_Order.size(); ++i){
		int cellId = nonEmpty_Order[i];
		int groupId = resGrouping[i];
		sufCell_CoarseSuf_marchingTet(cellId, groupId);

		// find segs to merge
		int count = 0;
		for (int cyci = 0; cyci < cell2cyc[cellId].size(); ++cyci){
			for (int si : cyc2seg[cell2cyc[cellId][cyci]]){
				seg2cells[si].push_back(cellId);
				segInCellPos[si].push_back(count);
				count++;
			}
		}
	}
#if _DEBUG_MING_MORE
	//for (size_t i=0; i<nonEmpty_Order.size(); ++i){
	//	//for (size_t i=0; i<cellEmpty.size(); ++i){
	//	//if(cellEmpty[i]) continue;
	//	int cellId = nonEmpty_Order[i];
	//	Utility::saveTilingObj((t_savedir+"ms_eachCell_befMS_"+Utility::toStr(t_genus)+"_"+Utility::toStr(cellId)+".obj").c_str(),allCellSufs.cells[cellId].sufV,allCellSufs.cells[cellId].sufF);
	//}
#endif
	//merge segs and subdivide/merge surfs
	sufCell_mergeSegsAndSubdivideSufs(seg2cells, segInCellPos);
	end = clock();
	times.push_back( (float)(end - start) / CLOCKS_PER_SEC );
#if _CG_LOG
	cout << "   Time: "<< times.back() << "s " << endl;
#endif

#if _SAVERES_MING
	sufCell_saveFinalSufForSmooth(0);
#endif

#if SUBTIMING
	Utility::writeVector(times, (t_savedir+"timesSuf.txt").c_str());
#endif
}

void Arrangement::sufCell_smoothSurface(){
	if(t_nSmBefLoop>0 || t_nLoop>0){
		cout << "\n-> Calling JuFair... ";
		sufCell_JuFair( t_nSmBefLoop, t_nLoop, t_nSmInLoop, true);
		cout << "DONE." << endl;
	}
}
void Arrangement::sufCell_saveFinalSufForSmooth(const int label){

	Utility::saveTilingObj((t_savedir+"Result.obj").c_str(), finalSuf.sufV, finalSuf.sufF);
	vector<vector<int> > tmpSegs;
	tmpSegs.reserve(finalSuf.sufSeg.size()*20);
	vector<int> oneSeg(2);
	for(int i=0; i<finalSuf.sufSeg.size(); ++i){
		for(int j=0; j<finalSuf.sufSeg[i].size()-1; ++j){
			oneSeg[0] = finalSuf.sufSeg[i][j];
			oneSeg[1] = finalSuf.sufSeg[i][j+1];
			tmpSegs.push_back(oneSeg);
		}
	}
	Utility::writeVectorForm(tmpSegs, (t_savedir+"ContuorEdges.txt").c_str());
	
	/*
	// save data for Nathan
	// Utility::saveTilingObj((t_savedir+"ForNate_"+Utility::toStr(label)+".obj").c_str(),finalSuf.sufV, finalSuf.sufF);
	vector<vector<int> > tmpSegs;
	tmpSegs.reserve(finalSuf.sufSeg.size()*20);
	vector<int> oneSeg(2);
	for(int i=0; i<finalSuf.sufSeg.size(); ++i){
		for(int j=0; j<finalSuf.sufSeg[i].size()-1; ++j){
			oneSeg[0] = finalSuf.sufSeg[i][j];
			oneSeg[1] = finalSuf.sufSeg[i][j+1];
			tmpSegs.push_back(oneSeg);
		}
	}
	// Utility::writeVectorForm(tmpSegs, (t_savedir+"ForNate_E_"+Utility::toStr(label)+".txt").c_str());
	bool *segs = new bool [finalSuf.sufV.size()];
	for(size_t i=0; i<finalSuf.sufSeg.size(); ++i){
		for(size_t j=0; j<finalSuf.sufSeg[i].size(); ++j){
			segs[finalSuf.sufSeg[i][j]] = true;
		}
	}

	vector<float> tmpScaleParas = unifyTransVec;
	tmpScaleParas.push_back(unifyScaleRatio);
	// Utility::writeVectorForm(tmpScaleParas,(t_savedir+"ForNate_Scale_"+Utility::toStr(label)+".txt").c_str());

	// save data for Lu
	// finalSuf.saveSuf((t_savedir+"ForLu_"+Utility::toStr(label)+".suf").c_str());
	// writeLuCtrGraph((t_savedir+"forLu.ctrGraph").c_str());
	*/
}
void Arrangement::sufCell_genSurface_Triangulation(vector<int> &resGrouping){

	// sufV
	finalSuf.sufV = Vs;

	// sufSeg
	vector<int> oneSeg(2);
	for(size_t si=0; si<seg2V.size(); ++si){
		for(size_t vi=0; vi<seg2V[si].size()-1; ++vi){
			oneSeg[0] = seg2V[si][vi];
			oneSeg[1] = seg2V[si][vi+1];
			finalSuf.sufSeg.push_back(oneSeg);
		}
	}

	// sufF
	getCyc2V();
	for (size_t i=0; i<nonEmpty_Order.size(); ++i){
		int cellId = nonEmpty_Order[i];
		int groupId = resGrouping[i];
		
		sufCell_TriangulateEachCell(cellId, groupId);
		
	}
	Utility::forceSufOriented(finalSuf.sufF);
	sufCell_saveFinalSufForSmooth(0);
	Utility::forceSufOriented(finalSuf.sufF);
	sufCell_saveFinalSufForSmooth(1);
}

void Arrangement::sufCell_mergeSegsAndSubdivideSufs(vector<vector<int> > &seg2cells, vector<vector<int> > &segInCellPos){
	// for each seg, find its cooresponding two appreance in 2 cells, sub seg and suf
	for(int si=0; si<nSeg; ++si){
		int cell0 = seg2cells[si][0];
		int cell1 = seg2cells[si][1];
		int pos0 = segInCellPos[si][0];
		int pos1 = segInCellPos[si][1];

		sufCell_mergeSubOneSegSuf(cell0, cell1, pos0, pos1);
	}

	// now segs and faces are merged within nb cells, ready for final merging globally
	vector<vector<float> > segIntersections(segGraph.size());
	vector<bool> segIntersectionsSet(segGraph.size(),false);
	vector<int> segFront2segIntersection(nSeg, -1);
	vector<int> segEnd2segIntersection(nSeg, -1);

	BigInt isOrgSegVIntersection(nSegV);
	for (int si = 0; si < nSeg; ++si){
		int vFront, vEnd;
		int v0, v1;
		vFront = seg2V[si].front();
		vEnd = seg2V[si].back();
		v0 = V2seggV[vFront];
		v1 = V2seggV[vEnd];
		if (v0 != v1){
			// two intersections
			segFront2segIntersection[si] = v0;
			segEnd2segIntersection[si] = v1;
			if(!segIntersectionsSet[v0]){
				segIntersections[v0] = Vs[vFront];
			}
			if(!segIntersectionsSet[v1]){
				segIntersections[v1] = Vs[vEnd];
			}
		}
	}

	// (1) store the intersections first
	finalSuf.sufV = segIntersections;

	// (2) store vertices in the interor of each seg
	vector<vector<int> > seg2sufV(nSeg);
	for(int si=0; si<nSeg; ++si){
		int cell0 = seg2cells[si][0];
		int pos0 = segInCellPos[si][0];
		vector<int> & curSufSeg = allCellSufs.cells[cell0].sufSeg[pos0];
		vector<vector<float> > & curSufV = allCellSufs.cells[cell0].sufV;
		int curNSegV = curSufSeg.size();
		seg2sufV[si].resize(curNSegV);
		finalSuf.sufV.reserve(finalSuf.sufV.size()+curNSegV);
		int vFront = segFront2segIntersection[si];
		int vEnd = segEnd2segIntersection[si];
		// first record/store the front and ends, if they are intersections, do not store
		if (vFront!=vEnd){
			seg2sufV[si][0] = vFront;
			seg2sufV[si][seg2sufV[si].size()-1] = vEnd;
		}else{
			seg2sufV[si][0] = finalSuf.sufV.size();
			seg2sufV[si][seg2sufV[si].size()-1] = seg2sufV[si][0];
			finalSuf.sufV.push_back(curSufV[curSufSeg[0]]);
		}
		// now record/store the interior of each seg
		for(int i=1; i<curSufSeg.size()-1; ++i){
			seg2sufV[si][i] = finalSuf.sufV.size();
			finalSuf.sufV.push_back(curSufV[curSufSeg[i]]);
		}
	}

	finalSuf.sufSeg = seg2sufV;

	// (3) process each cell, store other vertices and modify/store faces
	for (size_t i=0; i<nonEmpty_Order.size(); ++i){

		int cellId = nonEmpty_Order[i];

		unordered_map<int, int> locV2glbV;
		finalSuf.sufV.reserve(finalSuf.sufV.size()+allCellSufs.cells[cellId].sufV.size());
		finalSuf.sufF.reserve(finalSuf.sufF.size()+allCellSufs.cells[cellId].sufF.size());

		vector<bool> ignoreVs;
		vector<bool> ignoreFs;
		if(cellId == 11){
			int tmpdebug = 1;
		}

		sufCell_getRidOfNonManifold(allCellSufs.cells[cellId].sufV.size(), allCellSufs.cells[cellId].sufF, ignoreVs, ignoreFs);
		
		// (3.1) hash vertices on segs
		int segPos = 0;
		for (int cyci = 0; cyci < cell2cyc[cellId].size(); ++cyci){
			for (int si : cyc2seg[cell2cyc[cellId][cyci]]){
				for (int vi=0; vi < seg2sufV[si].size(); ++vi){
					locV2glbV[allCellSufs.cells[cellId].sufSeg[segPos][vi]] = seg2sufV[si][vi];
				}
				segPos++;
			}
		}

		// (3.2) hash other vertices
		for (int vi =0; vi<allCellSufs.cells[cellId].sufV.size(); ++vi){
			if(locV2glbV.find(vi)==locV2glbV.end()){
				if(ignoreVs[vi]) continue;
				locV2glbV[vi] = finalSuf.sufV.size();
				finalSuf.sufV.push_back(allCellSufs.cells[cellId].sufV[vi]);
			}
		}

		// (3.3) modify faces
		vector<vector<int> > curSufF = allCellSufs.cells[cellId].sufF;

		for(int fi=0; fi<curSufF.size(); ++fi){
			for(int j=0; j<3; ++j){
				curSufF[fi][j] = locV2glbV[curSufF[fi][j]];
			}
		}

		// (3.4) store faces
		for(int fi=0; fi<curSufF.size(); ++fi){
			if(ignoreFs[fi]) continue;
			finalSuf.sufF.push_back(curSufF[fi]);
		}
	}
}

// only get rid of two overlapping triangles
void Arrangement::sufCell_getRidOfNonManifold_simpleCase(int nSufV, vector<vector<int> > &sufF, vector<bool> &ignoreVs,vector<bool> &ignoreFs){
	//
}

// the overlapping-two-triangles case is a special case in the non-manifoldness after snapping
// the general case is that a piece of surface form a little bubble and connect to the main surface through a non-manifold edge of degree 4
// check the dual graph (no dual edge for non-manifold edge) and pick only the largest connected component, which is the main surface in primary mesh
void Arrangement::sufCell_getRidOfNonManifold(int nSufV, vector<vector<int> > &sufF, vector<bool> &ignoreVs,vector<bool> &ignoreFs){
	ignoreVs.resize(nSufV, false);
	ignoreFs.resize(sufF.size(), false);
	vector<int> FToDel;
	// find the edges E2V and edge-face connections E2nbF
	unordered_map<vector<int>, int, myVecHash> eHash;
	vector<vector<int> > E2V;
	vector<vector<int> > E2nbF;
	vector<int> oneE2V(2);
	for(int fi=0; fi<sufF.size(); ++fi){
		for(int i=0; i<3; ++i){
			oneE2V[0] = sufF[fi][i];
			oneE2V[1] = i == 2? sufF[fi][0] : sufF[fi][i+1];
			sort(oneE2V.begin(), oneE2V.end());
			//new edge is found
			if(eHash.find(oneE2V)==eHash.end()){
				eHash[oneE2V] = E2V.size();
				E2V.push_back(oneE2V);
				vector<int> oneE2nbF(1,fi);
				E2nbF.push_back(oneE2nbF);
			}else{
				E2nbF[eHash[oneE2V]].push_back(fi);
			}
		}
	}
	// construct the dual graph, and record the two ends of non-manifold edges
	// dGraph is the graph that seperate meshes at non-manifold edges
	// dGraph0 is the original mesh's dual graph (could be more than on component)
	UnDirGraph dGraph(sufF.size());
	unordered_set<int> nonManifoldVs;
	unordered_set<int> nonManifoldEs;
	for(int ei=0; ei<E2V.size(); ++ei){
		if(E2nbF[ei].size()==2){
			dGraph.addEdge(E2nbF[ei][0],E2nbF[ei][1]);
		}else{
			//ignore boundary triangles
			if(E2nbF[ei].size()!=1){
				nonManifoldVs.insert(E2V[ei][0]);
				nonManifoldVs.insert(E2V[ei][1]);
				nonManifoldEs.insert(ei);
			}
		}
	}
	// label connected components of the dual graph
	vector<int> F2CompID;
	vector<int> FCompSize;
	vector<int> FSeeds;
	
	dGraph.labelConnectedComponents(F2CompID,FCompSize); 
	
	UnDirGraph tinyCompGraph(FCompSize.size());
	for(auto ei : nonManifoldEs){
		for(int i=0; i<E2nbF[ei].size()-1; ++i){
			for(int j=i+1; j<E2nbF[ei].size(); ++j){
				tinyCompGraph.addEdge(F2CompID[E2nbF[ei][i]],F2CompID[E2nbF[ei][j]]);
			}
		}
	}
	vector<vector<int> > suf2comps;
	tinyCompGraph.findConnectedComponents(suf2comps);

	vector<int> suf2LCompID(suf2comps.size());
	for(int sufi=0; sufi<suf2comps.size(); ++sufi){
		// if there is only one component, this is already the largest,
		int LargestCompSize = -1;
		for(auto compID : suf2comps[sufi]){
			if(FCompSize[compID]>LargestCompSize){
				LargestCompSize = FCompSize[compID];
				suf2LCompID[sufi] = compID;
			}
		}
	}
	unordered_set<int> validCompIDs(suf2LCompID.begin(),suf2LCompID.end());
	vector<vector<int> > comp2Fs(FCompSize.size());
	for(int fi=0; fi<F2CompID.size(); ++fi){
		// only find the surfaces of small component
		if(validCompIDs.find(F2CompID[fi])==validCompIDs.end()){
			for(int i=0; i<3; ++i){
				oneE2V[0] = sufF[fi][i];
				oneE2V[1] = i == 2? sufF[fi][0] : sufF[fi][i+1];
				sort(oneE2V.begin(), oneE2V.end());
				// if any triangle in this component is a boundary face (using boundary edge),
				// then this component must be saved
				if(E2nbF[eHash[oneE2V]].size()==1){
					validCompIDs.insert(F2CompID[fi]);
					break;
				}
			}
		}
	}
	for(size_t fi=0; fi<F2CompID.size(); ++fi){
		if(validCompIDs.find(F2CompID[fi])==validCompIDs.end()){
			ignoreFs[fi] = true;
			FToDel.push_back(fi);
		}
	}
	for(auto fi : FToDel){
		vector<int> &oneTrianlge = sufF[fi];
		for(int vi : oneTrianlge){
			// leave the two ends of the nonManifold edges out, for the main surface
			if(nonManifoldVs.find(vi)==nonManifoldVs.end()){
				ignoreVs[vi] = true;
			};
		}
	}
}

void Arrangement::sufCell_mergeSubOneSegSuf(int cell0, int cell1, int pos0, int pos1){
	vector<int> &sufSeg0 = allCellSufs.cells[cell0].sufSeg[pos0];
	vector<int> &sufSeg1 = allCellSufs.cells[cell1].sufSeg[pos1];
	int vn0 = sufSeg0.size();
	int vn1 = sufSeg1.size();
	vector<float> distList0;
	distList0.reserve(vn0-2);
	vector<float> distList1;
	distList1.reserve(vn1-2);
	vector<vector<int> > merge0;
	vector<vector<int> > merge1;

	// find the distance of each interior seg v (n-2 interior v) to the first seg v
	float len, dist=0.0f;
	float minLen=INFINITY;
	for(int i=1; i<vn0-1; ++i){
		Eigen::Vector3f vec(
			allCellSufs.cells[cell0].sufV[sufSeg0[i-1]][0] - allCellSufs.cells[cell0].sufV[sufSeg0[i]][0],
			allCellSufs.cells[cell0].sufV[sufSeg0[i-1]][1] - allCellSufs.cells[cell0].sufV[sufSeg0[i]][1],
			allCellSufs.cells[cell0].sufV[sufSeg0[i-1]][2] - allCellSufs.cells[cell0].sufV[sufSeg0[i]][2]);
		len=vec.norm();
		dist+=len;
		distList0.push_back(dist);
		minLen = len < minLen ? len : minLen;
	}

	dist=0.0f;
	for(int i=1; i<vn1-1; ++i){
		Eigen::Vector3f vec(
			allCellSufs.cells[cell1].sufV[sufSeg1[i-1]][0] - allCellSufs.cells[cell1].sufV[sufSeg1[i]][0],
			allCellSufs.cells[cell1].sufV[sufSeg1[i-1]][1] - allCellSufs.cells[cell1].sufV[sufSeg1[i]][1],
			allCellSufs.cells[cell1].sufV[sufSeg1[i-1]][2] - allCellSufs.cells[cell1].sufV[sufSeg1[i]][2]);
		len=vec.norm();
		dist+=len;
		distList1.push_back(dist);
		minLen = len < minLen ? len : minLen;
	}

	// find the interval-merges of the two segments
	Utility::findMergingOfTwoLists(distList0,distList1,merge0, merge1,minLen/1000);

	int addN0 = 0;
	int addN1 = 0;
	for (int i=0; i<merge0.size(); ++i){
		addN0 += merge0[i].size();
	}
	for (int i=0; i<merge1.size(); ++i){
		addN1 += merge1[i].size();
	}

	allCellSufs.cells[cell0].sufV.reserve(allCellSufs.cells[cell0].sufV.size() + addN0);
	allCellSufs.cells[cell1].sufV.reserve(allCellSufs.cells[cell1].sufV.size() + addN1);

	vector<int> mergedSufSeg0;
	mergedSufSeg0.reserve(sufSeg0.size()+addN0);
	vector<int> mergedSufSeg1;
	mergedSufSeg1.reserve(sufSeg1.size()+addN1);
	sufCell_subOneSegSuf(
		allCellSufs.cells[cell0].sufV,
		allCellSufs.cells[cell1].sufV,
		allCellSufs.cells[cell0].sufF,
		allCellSufs.cells[cell0].sufSeg2F[pos0],
		sufSeg0,
		sufSeg1,
		merge0,
		mergedSufSeg0);
	sufCell_subOneSegSuf(
		allCellSufs.cells[cell1].sufV,
		allCellSufs.cells[cell0].sufV,
		allCellSufs.cells[cell1].sufF,
		allCellSufs.cells[cell1].sufSeg2F[pos1],
		sufSeg1,
		sufSeg0,
		merge1,
		mergedSufSeg1);
	allCellSufs.cells[cell0].sufSeg[pos0] = mergedSufSeg0;
	allCellSufs.cells[cell1].sufSeg[pos1] = mergedSufSeg1;

	
}
void Arrangement::sufCell_subOneSegSuf(vector<vector<float> > &sufV0, vector<vector<float> > &sufV1, vector<vector<int> > &sufF0, vector<int> &sufSeg2F0, vector<int> &sufSeg0, vector<int> &sufSeg1,vector<vector<int> > &merge0, vector<int> &mergedSufSeg0){
	vector<int> oneFace(3);
	// for each segment in the boundary, insert points from the dual boundary
	int curFi, vz, addvi, curVN = sufV0.size(), curAddN = 0;
	int vzPos=-1, v0Pos=-1, v1Pos=-1;

	for(int i=0; i<sufSeg0.size()-1; ++i){
		mergedSufSeg0.push_back(sufSeg0[i]);

		if (merge0[i].size()==0) continue;
		
		curFi = sufSeg2F0[i];
		vector<int> &curF = sufF0[curFi];
		vz = curF[0] + curF[1] +curF[2] - sufSeg0[i] - sufSeg0[i+1];

		// find positions of each vertices, to keep the orientation of the subdivided surface
		for(int j=0; j<3; ++j){
			if(curF[j] == vz){
				vzPos = j;
			}else if(curF[j] == sufSeg0[i]){
				v0Pos = j;
			}else if(curF[j] == sufSeg0[i+1]){
				v1Pos = j;
			}else{
				cout << "ERROR! in subOneSegSuf! " << endl;
			}
		}

		// first
		oneFace[vzPos] = vz;
		oneFace[v0Pos] = sufSeg0[i];
		oneFace[v1Pos] = sufV0.size();
		sufF0.push_back(oneFace);
		// last
		sufF0[curFi][vzPos] = vz;
		sufF0[curFi][v1Pos] = sufSeg0[i+1];
		sufF0[curFi][v0Pos] = sufV0.size()+merge0[i].size()-1;

		for(int j=0; j<merge0[i].size()-1; ++j){
			addvi = sufSeg1[merge0[i][j]+1];
			mergedSufSeg0.push_back(sufV0.size());
			sufV0.push_back(sufV1[addvi]);

			oneFace[v0Pos] = sufV0.size()-1;//curVN+j;
			oneFace[v1Pos] = sufV0.size();//curVN+j+1;
			sufF0.push_back(oneFace);
		}
		addvi = sufSeg1[merge0[i].back()+1];
		mergedSufSeg0.push_back(sufV0.size());
		sufV0.push_back(sufV1[addvi]);
	}
	mergedSufSeg0.push_back(sufSeg0.back());
}
bool Arrangement::sufCell_isTetAct(int ci, int ti, float iso){
	if (
		(
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[0]]>=iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[1]]>=iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[2]]>=iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[3]]>=iso)

		)||
		(
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[0]]<iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[1]]<iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[2]]<iso)&&
		(allCellTets.cells[ci].VPro[allCellTets.cells[ci].cellTets[ti].v[3]]<iso)
		)){
			return false;
	}else{
		return true;
	}
}

void Arrangement::sufCell_marchingTet_calcInterpolateV(int cellId, int ei, float iso, vector<int> &tetE2sufV, vector<vector<float> > &sufV, unordered_map<int, int> &segV2sufV/*, int vi*/){
	int v0 = allCellTets.cells[cellId].cellEdges[ei].v[0];
	int v1 = allCellTets.cells[cellId].cellEdges[ei].v[1];
	vector<float> oneV(3);
	//only snap if the new vertices are on the boundary facets, do not do that for interior edges
	if(allCellTets.cells[cellId].cellEdges[ei].EinoutMark!=TagMapping::MAT_NAN){
		if (TagMapping::isTetVsOnSeg(allCellTets.cells[cellId].cellVertices[v0].VinoutMark)){
			if(segV2sufV.find(v0)==segV2sufV.end()){
				oneV[0] = allCellTets.cells[cellId].cellVertices[v0].xyz[0];
				oneV[1] = allCellTets.cells[cellId].cellVertices[v0].xyz[1];
				oneV[2] = allCellTets.cells[cellId].cellVertices[v0].xyz[2];
				tetE2sufV[ei] = sufV.size();
				segV2sufV[v0] = tetE2sufV[ei];
				sufV.push_back(oneV);

			}else{
				tetE2sufV[ei] = segV2sufV[v0];
			}
			return;
		} 
		if (TagMapping::isTetVsOnSeg(allCellTets.cells[cellId].cellVertices[v1].VinoutMark)){
			if(segV2sufV.find(v1)==segV2sufV.end()){
				oneV[0] = allCellTets.cells[cellId].cellVertices[v1].xyz[0];
				oneV[1] = allCellTets.cells[cellId].cellVertices[v1].xyz[1];
				oneV[2] = allCellTets.cells[cellId].cellVertices[v1].xyz[2];
				tetE2sufV[ei] = sufV.size();
				segV2sufV[v1] = tetE2sufV[ei];
				sufV.push_back(oneV);

			}else{
				tetE2sufV[ei] = segV2sufV[v1];
			}
			return;
		} 
	}
	float iso0 = allCellTets.cells[cellId].VPro[v0];
	float iso1 = allCellTets.cells[cellId].VPro[v1];
	Eigen::Vector3f coord0((float)allCellTets.cells[cellId].cellVertices[v0].xyz[0], (float)allCellTets.cells[cellId].cellVertices[v0].xyz[1], (float)allCellTets.cells[cellId].cellVertices[v0].xyz[2]);
	Eigen::Vector3f coord1((float)allCellTets.cells[cellId].cellVertices[v1].xyz[0], (float)allCellTets.cells[cellId].cellVertices[v1].xyz[1], (float)allCellTets.cells[cellId].cellVertices[v1].xyz[2]);

	Eigen::Vector3f v;

	v = coord0 + ((iso0-iso)/(iso0-iso1))*(coord1-coord0);
	oneV[0] = v[0];
	oneV[1] = v[1];
	oneV[2] = v[2];
	tetE2sufV[ei] = sufV.size();
	sufV.push_back(oneV);
}

void Arrangement::sufCell_marchingTet_calcInterpolateV(int cellId, int ei, float iso, unordered_map<int,int> &tetE2sufV, vector<vector<float> > &sufV, unordered_map<int, int> &segV2sufV/*, int vi*/){
	int v0 = allCellTets.cells[cellId].cellEdges[ei].v[0];
	int v1 = allCellTets.cells[cellId].cellEdges[ei].v[1];
	vector<float> oneV(3);
	//only snap if the new vertices are on the boundary facets, do not do that for interior edges
	if(allCellTets.cells[cellId].cellEdges[ei].EinoutMark!=TagMapping::MAT_NAN){
		if (TagMapping::isTetVsOnSeg(allCellTets.cells[cellId].cellVertices[v0].VinoutMark)){
			if(segV2sufV.find(v0)==segV2sufV.end()){
				oneV[0] = allCellTets.cells[cellId].cellVertices[v0].xyz[0];
				oneV[1] = allCellTets.cells[cellId].cellVertices[v0].xyz[1];
				oneV[2] = allCellTets.cells[cellId].cellVertices[v0].xyz[2];
				tetE2sufV[ei] = sufV.size();
				segV2sufV[v0] = sufV.size();
				sufV.push_back(oneV);

			}else{
				tetE2sufV[ei] = segV2sufV[v0];
			}
			return;
		} 
		if (TagMapping::isTetVsOnSeg(allCellTets.cells[cellId].cellVertices[v1].VinoutMark)){
			if(segV2sufV.find(v1)==segV2sufV.end()){
				oneV[0] = allCellTets.cells[cellId].cellVertices[v1].xyz[0];
				oneV[1] = allCellTets.cells[cellId].cellVertices[v1].xyz[1];
				oneV[2] = allCellTets.cells[cellId].cellVertices[v1].xyz[2];
				tetE2sufV[ei] = sufV.size();
				segV2sufV[v1] = sufV.size();
				sufV.push_back(oneV);

			}else{
				tetE2sufV[ei] = segV2sufV[v1];
			}
			return;
		} 
	}
	float iso0 = allCellTets.cells[cellId].VPro[v0];
	float iso1 = allCellTets.cells[cellId].VPro[v1];
	Eigen::Vector3f coord0((float)allCellTets.cells[cellId].cellVertices[v0].xyz[0], (float)allCellTets.cells[cellId].cellVertices[v0].xyz[1], (float)allCellTets.cells[cellId].cellVertices[v0].xyz[2]);
	Eigen::Vector3f coord1((float)allCellTets.cells[cellId].cellVertices[v1].xyz[0], (float)allCellTets.cells[cellId].cellVertices[v1].xyz[1], (float)allCellTets.cells[cellId].cellVertices[v1].xyz[2]);

	Eigen::Vector3f v;

	v = coord0 + ((iso0-iso)/(iso0-iso1))*(coord1-coord0);
	oneV[0] = v[0];
	oneV[1] = v[1];
	oneV[2] = v[2];
	tetE2sufV[ei] = sufV.size();
	sufV.push_back(oneV);
}

void Arrangement::sufCell_CoarseSuf_TMP(int cellId, int groupId){
	vector<vector<vector<float> > > inCurves;
	vector<vector<vector<float> > > inNorms;
	bool useDelaunay = true;
	bool useMinSet = true;
	bool useNormal = false;
	float areaWeight = 0.0f;
	float edgeWeight = 0.0f;
	float dihedralWeight = 1.0f;
	float boundaryNormalWeight = 1.0f;
	int _numofpoints;
	int _numoftilingtris;
	float * _pPositions;
	float * _pNormals;
	int * _pFaceIndices;

	vector<vector<float> > oneSetSufV;
	vector<vector<int> > oneSetSufF;
	vector<vector<float> > & curCellSufV = allCellSufs.cells[cellId].sufV;
	vector<vector<int> > & curCellSufF = allCellSufs.cells[cellId].sufF;
	vector<vector<int> > & curCellSufSeg = allCellSufs.cells[cellId].sufSeg;
	vector<vector<int> > & curCellSufSeg2F = allCellSufs.cells[cellId].sufSeg2F;
	
	vector<int> grouping = allGroupings[cellId][groupId];
	for (size_t gi=0; gi<grouping.size(); ++gi){
		vector<int> oneSetCycs = allSets[grouping[gi]];
		int curCycInd;
		inCurves.clear();
		oneSetSufV.clear();
		oneSetSufF.clear();
		inCurves.resize(oneSetCycs.size());
		for (size_t osi=0; osi<oneSetCycs.size(); ++osi){
			curCycInd = cell2cyc[cellId][oneSetCycs[osi]];
			vector<int> & curCyc2V = cyc2V[curCycInd];
			inCurves[osi].reserve(curCyc2V.size());
			for(auto vi : curCyc2V){
				inCurves[osi].push_back(Vs[vi]);
				oneSetSufV.push_back(Vs[vi]);
			}
		}

		sufCell_MingTriMultPoly(inCurves, inNorms, useDelaunay, useMinSet, useNormal, 
			areaWeight, edgeWeight, dihedralWeight, boundaryNormalWeight, 
			oneSetSufF,cellId, groupId);

		Utility::saveTilingObj((t_savedir+"triSufCur"+Utility::toStr(cellId)+"_"+Utility::toStr(gi)+".obj").c_str(), oneSetSufV, oneSetSufF);
		
	}
}
void Arrangement::sufCell_TriangulateEachCell(int cellId, int groupId){
	vector<int> inCurvesInd;
	vector<vector<vector<float> > > inCurves;
	vector<vector<vector<float> > > inNorms;
	bool useDelaunay = true;
	bool useMinSet = true;
	bool useNormal = false;
	float areaWeight = 0.0f;
	float edgeWeight = 0.0f;
	float dihedralWeight = 0.0f;
	float boundaryNormalWeight = 1.0f;
	int _numofpoints;
	int _numoftilingtris;
	float * _pPositions;
	float * _pNormals;
	int * _pFaceIndices;

	vector<vector<float> > oneSetSufV;
	vector<vector<int> > oneSetSufF;
	
	vector<int> oneF(3);
	vector<int> grouping = allGroupings[cellId][groupId];
	for (size_t gi=0; gi<grouping.size(); ++gi){
		vector<int> oneSetCycs = allSets[grouping[gi]];
		int curCycInd;
		inCurves.clear();
		inCurvesInd.clear();
		oneSetSufV.clear();
		oneSetSufF.clear();
		inCurves.resize(oneSetCycs.size());
		
		for (size_t osi=0; osi<oneSetCycs.size(); ++osi){
			curCycInd = cell2cyc[cellId][oneSetCycs[osi]];
			vector<int> & curCyc2V = cyc2V[curCycInd];
			inCurves[osi].reserve(curCyc2V.size());
			for(auto vi : curCyc2V){
				inCurves[osi].push_back(Vs[vi]);
				oneSetSufV.push_back(Vs[vi]);
				inCurvesInd.push_back(vi);
				
			}
		}
		Utility::saveTriangulationCurveFile((t_savedir+"triSufBef"+Utility::toStr(cellId)+"_"+Utility::toStr(gi)+".curve").c_str(), inCurves);

		sufCell_MingTriMultPoly(inCurves, inNorms, useDelaunay, useMinSet, useNormal, 
			areaWeight, edgeWeight, dihedralWeight, boundaryNormalWeight, 
			oneSetSufF,cellId, groupId);
		// check if triangulation did not add additional points, if it does, cannot use this.
		cout << "save file: " << (t_savedir+"triSufCur"+Utility::toStr(cellId)+"_"+Utility::toStr(gi)+".obj") << endl;
		Utility::saveTilingObj((t_savedir+"triSufCur"+Utility::toStr(cellId)+"_"+Utility::toStr(gi)+".obj").c_str(), oneSetSufV, oneSetSufF);

		for(auto & _oneF : oneSetSufF){
			oneF[0] = inCurvesInd[_oneF[0]];
			oneF[1] = inCurvesInd[_oneF[1]];
			oneF[2] = inCurvesInd[_oneF[2]];
			finalSuf.sufF.push_back(oneF);
		}
		
	}
}
void Arrangement::getCyc2V(){
	cyc2V.resize(cyc2seg.size());
	for(size_t ci = 0; ci < cyc2seg.size(); ++ci){
		vector<int> oneCyc;
		vector<int> oneSeg;
		// if current seg/cyc is closed, push the v indexes in directly, but get rid of one end
		if(cyc2seg[ci].size()==1){
			cyc2V[ci]=(seg2V[cyc2seg[ci][0]]);
			cyc2V[ci].pop_back(); // there is a duplicate end for closed segment, get rid of it
		}else{
			int startV = seg2V[cyc2seg[ci][0]][0]; // the first V of the first seg
			int curEndV = startV;
			for(size_t si = 0; si < cyc2seg[ci].size(); ++si){
				oneSeg = seg2V[si];
				if(seg2V[si][0] != curEndV){
					reverse(oneSeg.begin(), oneSeg.end());
				}
				curEndV = oneSeg.back();
				oneSeg.pop_back();
				oneCyc.insert(oneCyc.end(), oneSeg.begin(), oneSeg.end());
			}
			cyc2V[ci] = oneCyc;
		}
	}
}
void Arrangement::sufCell_CoarseSuf_marchingTet(int cellId, int groupId){
	vector<float> mix_isos;
	vector<BigInt> mix_actEdges;

	float curIso = allGroupingIso[cellId][groupId];

	// if curIso is valid (not mix-iso case), then there is only one iso value and one corresponding set of active edges
	// otherwise, save the different iso values and the different sets of active edges in vector, for later processing, one by one
	if(curIso==MIXISOFAKEVALUE){
		unordered_map<int,int> globalSet2locSet;
		for(int i=0; i<cell2Loc_Global_setMap[cellId].size(); ++i){
			globalSet2locSet[cell2Loc_Global_setMap[cellId][i]] = i;
		}
		int nSubGroup = allGroupings[cellId][groupId].size();
		mix_isos.reserve(nSubGroup);
		mix_actEdges.reserve(nSubGroup);
		for(auto global_subGroupID : allGroupings[cellId][groupId]){
			mix_isos.push_back(cell2Loc_setIso[cellId][globalSet2locSet[global_subGroupID]]);
			mix_actEdges.push_back(cell2Loc_setActEdge[cellId][globalSet2locSet[global_subGroupID]]);
		}
	}else{
		mix_isos.push_back(curIso);
		BigInt singleIsoActEdge(allCellTets.cells[cellId].cellEdges.size());
		tetCell_tagActiveEdge(cellId, curIso, singleIsoActEdge);
		mix_actEdges.push_back(singleIsoActEdge);
	}

	// goal date structure to construct for all mixed iso values
	unordered_map<int, int> segV2sufV;
	vector<vector<float> > &sufV = allCellSufs.cells[cellId].sufV;
	vector<vector<int> > &sufF = allCellSufs.cells[cellId].sufF;

	vector<unordered_map<int,int> > mix_TetE2SufVs(mix_isos.size());
	for(int i=0; i<mix_isos.size(); ++i){
		float iso = mix_isos[i];
		BigInt actEdges = mix_actEdges[i];
		unordered_map<int,int> &tetE2sufV = mix_TetE2SufVs[i];
		while(!actEdges.isZero()){
			sufCell_marchingTet_calcInterpolateV(cellId, actEdges.popOne(), iso, tetE2sufV, sufV, segV2sufV);
		}
	}

	sufCell_findSegsInSufV(cellId, segV2sufV);
	BigInt isSufVASegV(sufV.size());
	for(int i=0; i<allCellSufs.cells[cellId].sufSeg.size(); ++i){
		for(int j=0; j<allCellSufs.cells[cellId].sufSeg[i].size(); ++j){			
			isSufVASegV.set(allCellSufs.cells[cellId].sufSeg[i][j]);
		}
	}
	
	unordered_map<vector<int>, int, myVecHash> SegE2Fhash;

	vector<int> oneTriangle(3);
	
	for(int i=0; i<mix_isos.size(); ++i){
		float iso = mix_isos[i];
		BigInt & actEdge = mix_actEdges[i];
		unordered_map<int,int> &tetE2sufV = mix_TetE2SufVs[i];

		// find active tets, tets are active iff they surround an active edge
		BigInt actTet(allCellTets.cells[cellId].cellTets.size());
		BigInt actEdgeCopy = actEdge;
		while(!actEdgeCopy.isZero()){
			int curEdge = actEdgeCopy.popOne();
			for(auto nbF : allCellTets.cells[cellId].cellEdges[curEdge].nbFace){
				for(auto nbTet : allCellTets.cells[cellId].cellFaces[nbF].nbTet){
					if(nbTet!=-1){
						actTet.set(nbTet);
					}
				}
			}
		}

		while(!actTet.isZero()){
			int curTet = actTet.popOne();
			unordered_set<int> edges;
			for (int fi=0; fi<4; ++fi){
				for (int ei=0; ei<3; ++ei){
					edges.insert(allCellTets.cells[cellId].cellFaces[allCellTets.cells[cellId].cellTets[curTet].face[fi]].edge[ei]);
				}
			}
			vector<int> oneFace;
			for (auto it=edges.begin(); it!=edges.end(); ++it){
				if (actEdge.get(*it)){
					oneFace.push_back(*it);
				}
			}

			if (oneFace.size()==4){
				int e0 = oneFace[0];
				int e1 = oneFace[1];
				int e3 = oneFace[3];
				if (allCellTets.cells[cellId].cellEdges[e1].v[0]!=allCellTets.cells[cellId].cellEdges[e0].v[0]&& 
					allCellTets.cells[cellId].cellEdges[e1].v[0]!=allCellTets.cells[cellId].cellEdges[e0].v[1]&& 
					allCellTets.cells[cellId].cellEdges[e1].v[1]!=allCellTets.cells[cellId].cellEdges[e0].v[0]&& 
					allCellTets.cells[cellId].cellEdges[e1].v[1]!=allCellTets.cells[cellId].cellEdges[e0].v[1]){
						oneFace[1] = oneFace[2];
						oneFace[2] = e1;
				}else if (allCellTets.cells[cellId].cellEdges[e3].v[0]!=allCellTets.cells[cellId].cellEdges[e0].v[0]&& 
					allCellTets.cells[cellId].cellEdges[e3].v[0]!=allCellTets.cells[cellId].cellEdges[e0].v[1]&& 
					allCellTets.cells[cellId].cellEdges[e3].v[1]!=allCellTets.cells[cellId].cellEdges[e0].v[0]&& 
					allCellTets.cells[cellId].cellEdges[e3].v[1]!=allCellTets.cells[cellId].cellEdges[e0].v[1]){
						oneFace[3] = oneFace[2];
						oneFace[2] = e3;
				}

				//first face
				oneTriangle[0] = tetE2sufV[oneFace[0]];
				oneTriangle[1] = tetE2sufV[oneFace[1]];
				oneTriangle[2] = tetE2sufV[oneFace[2]];
				if(oneTriangle[0]!=oneTriangle[1] && oneTriangle[0]!=oneTriangle[2] && oneTriangle[2]!=oneTriangle[1]){

					sort(oneTriangle.begin(), oneTriangle.end());
					sufCell_hashSegFaces(oneTriangle,isSufVASegV,SegE2Fhash,sufF);

					// orient triangle
					if(!sufCell_isTriOriented(cellId,curTet,iso,sufV[oneTriangle[0]],sufV[oneTriangle[1]],sufV[oneTriangle[2]])){
						int tmpV = oneTriangle[0];
						oneTriangle[0] = oneTriangle[1];
						oneTriangle[1] = tmpV;
					}
					sufF.push_back(oneTriangle);

				}
				//second face
				oneTriangle[0] = tetE2sufV[oneFace[2]];
				oneTriangle[1] = tetE2sufV[oneFace[3]];
				oneTriangle[2] = tetE2sufV[oneFace[0]];
				if(oneTriangle[0]!=oneTriangle[1] && oneTriangle[0]!=oneTriangle[2] && oneTriangle[2]!=oneTriangle[1]){

					sort(oneTriangle.begin(), oneTriangle.end());
					sufCell_hashSegFaces(oneTriangle,isSufVASegV,SegE2Fhash,sufF);

					// orient triangle
					if(!sufCell_isTriOriented(cellId,curTet,iso,sufV[oneTriangle[0]],sufV[oneTriangle[1]],sufV[oneTriangle[2]])){
						int tmpV = oneTriangle[0];
						oneTriangle[0] = oneTriangle[1];
						oneTriangle[1] = tmpV;
					}
					sufF.push_back(oneTriangle);
				}
			} else {
				oneTriangle[0] = tetE2sufV[oneFace[0]];
				oneTriangle[1] = tetE2sufV[oneFace[1]];
				oneTriangle[2] = tetE2sufV[oneFace[2]];
				if(oneTriangle[0]!=oneTriangle[1] && oneTriangle[0]!=oneTriangle[2] && oneTriangle[2]!=oneTriangle[1]){
					sort(oneTriangle.begin(), oneTriangle.end());
					sufCell_hashSegFaces(oneTriangle,isSufVASegV,SegE2Fhash,sufF);

					// orient triangle
					if(!sufCell_isTriOriented(cellId,curTet,iso,sufV[oneTriangle[0]],sufV[oneTriangle[1]],sufV[oneTriangle[2]])){
						int tmpV = oneTriangle[0];
						oneTriangle[0] = oneTriangle[1];
						oneTriangle[1] = tmpV;
					} 
					sufF.push_back(oneTriangle);
				}
			}
		}
	}

	allCellSufs.cells[cellId].sufSeg2F = allCellSufs.cells[cellId].sufSeg;
	for(int i=0; i<allCellSufs.cells[cellId].sufSeg.size(); ++i){
		for(int j=0; j<allCellSufs.cells[cellId].sufSeg[i].size()-1; ++j){
			vector<int> oneHashEdge(2);
			oneHashEdge[0] = allCellSufs.cells[cellId].sufSeg[i][j];
			oneHashEdge[1] = allCellSufs.cells[cellId].sufSeg[i][j+1];
			sort(oneHashEdge.begin(), oneHashEdge.end());

			allCellSufs.cells[cellId].sufSeg2F[i][j] = SegE2Fhash[oneHashEdge];
		}
		allCellSufs.cells[cellId].sufSeg2F[i].pop_back();
	}
}

// extract the segs, but in index of the SufV, instead of TetV
void Arrangement::sufCell_findSegsInSufV(int cellId, unordered_map<int, int> &segV2sufV/*, vector<vector<int> > &cellSeg2SufV*/){
	allCellSufs.cells[cellId].sufSeg = allCellTets.cells[cellId].cellSeg2TetV;
	for(int i=0; i<allCellTets.cells[cellId].cellSeg2TetV.size(); ++i){
		for(int j=0; j<allCellTets.cells[cellId].cellSeg2TetV[i].size(); ++j){
			allCellSufs.cells[cellId].sufSeg[i][j] = segV2sufV[allCellTets.cells[cellId].cellSeg2TetV[i][j]];
		}
	}
}
void Arrangement::sufCell_SmoothPatch( const int _nSub, const int _lapOrder, const int _lapIter ){
	// MING: requires Adobe SmoothPatch libraries.
	//       use JuFair instead.
	// 
	////// (1) prepare the input Mesh: Vs, Fs, constraintVs
	////cout << "preparing the inputMesh... " << endl;
	////SP::Mesh inputMesh, outputMesh;
	////inputMesh.allocateVertices(finalSuf.sufV.size());
	////inputMesh.allocateFaces(finalSuf.sufF.size());

	////float *vertices   = inputMesh.getPositions();  // array 3 times the size of number of vertices¡ªaccess vertex position data
	////int *faces     = inputMesh.getFaceIndices();   // array 3 times the size of the number of faces (specifies triangle indices into vertex array
	////bool *segs    = inputMesh.getConstraints();    // array that is the size of the number of vertices.. set TRUE if you want to constrain that vertex curing smoothing

	////Utility::flatten2DVectors(finalSuf.sufV, vertices);
	////Utility::flatten2DVectors(finalSuf.sufF, faces);
	////for(size_t i=0; i<finalSuf.sufSeg.size(); ++i){
	////	for(size_t j=0; j<finalSuf.sufSeg[i].size(); ++j){
	////		segs[finalSuf.sufSeg[i][j]] = true;
	////	}
	////}
	////

	////// (2) change the smooth settings
	////cout << "change settings... " << endl;
	//////mnLaplacianOrder =  1 (Laplace), 2 (BiLaplace) , 3 (TriLaplace), 4 (QuadLaplace), ... 
	////SP::SurfaceSmoothSettings settings;
	////settings.mnNumSubdivisions=_nSub;
	////settings.mnLaplacianOrder=_lapOrder;
	////settings.mnLaplaceIterations=_lapIter;

	////vector<int> segEdges;
	////Utility::flatten2DVectors(finalSuf.sufSeg, segEdges);

	////cout << "save obj before smoothing... " << endl;
	////Utility::saveTilingObj((t_savedir+"PatchSmt_Bef_"+Utility::toStr(t_genus)+".obj").c_str(), inputMesh.getVertexCount(), inputMesh.getPositions(), inputMesh.getFaceCount(), inputMesh.getFaceIndices());
	//////Utility::writeAnArray((t_savedir+"PatchSmt_Bef_"+Utility::toStr(t_genus)+".txt").c_str(),segs,finalSuf.sufV.size());
	////cout << "save txt before smoothing... " << endl;
	////Utility::writeVectorForm(finalSuf.sufSeg, (t_savedir+"PatchSmt_Bef_"+Utility::toStr(t_genus)+".txt").c_str());

	////cout << "end... " << endl;

	////// (3) run!
	//////cout << "smoothing... " << endl;
	//////SP::SurfaceSmoother::smoothSurface(settings,inputMesh,outputMesh);

	////// (4) save results
	//////cout << "save obj after smoothing... " << endl;
	//////Utility::saveTilingObj((t_savedir+"finalSuf_PatchSmt_"+Utility::toStr(t_genus)+".obj").c_str(), outputMesh.getVertexCount(), outputMesh.getPositions(), outputMesh.getFaceCount(), outputMesh.getFaceIndices());

}
void Arrangement::sufCell_JuFair( int t_nSmBefLoop, int t_nLoop, int t_nSmInLoop, bool m_doSwap){
	double alphan = 1.414;
	double alpha0 = 0;

	double delta = (alphan - alpha0)/(t_nLoop==0?1:t_nLoop);
	double alpha = alpha0;

	double ratio = 0.5;

	vector<float> m_verLu;
	intvector m_faceLu;
	intvector m_ctrmedgeLu;

	Utility::flatten2DVectors(finalSuf.sufV, m_verLu);
	Utility::flatten2DVectors(finalSuf.sufF, m_faceLu);
	Utility::flatten2DVectors(finalSuf.sufSeg, m_ctrmedgeLu);

	LuMesh m_meshFair;
	m_meshFair.InputData(m_verLu,m_faceLu,m_ctrmedgeLu, m_doSwap);

	m_meshFair.JUFair( ratio, t_nSmBefLoop);
	for( int i = 0; i < t_nLoop; i ++)
	{
		alpha += delta;
		m_meshFair.LiepaRefine(alpha);
		m_meshFair.JUFair( ratio, t_nSmInLoop);
		
	}

	m_verLu.clear();
	m_faceLu.clear();
	m_ctrmedgeLu.clear();
	m_meshFair.OutputData(m_verLu,m_faceLu);

	Utility::scaleDnVers(m_verLu, unifyScaleRatio, unifyTransVec);
#if _SAVERES_MING
	Utility::saveTilingObj((t_savedir+"SmoothTao"/*+Utility::toStr(t_genus)*/+".obj").c_str(),m_verLu,m_faceLu);
#endif

	Utility::saveTilingObj((t_savedir+"Result_afterSmooth.obj").c_str(), m_verLu, m_faceLu);
}

void Arrangement::sufCell_findAnchorV(int cellId, int curTet, float iso, int &anchorV, bool &isAnchorVPositive){
	vector<float> isos(4);
	float v_iso;
	float maxDiff = -1.0f;
	int maxV = -1;
	float curDiff;
	int curV;
	for(int i=0; i<4; ++i){
		curV = allCellTets.cells[cellId].cellTets[curTet].v[i];
		v_iso = allCellTets.cells[cellId].VPro[curV];
		curDiff = abs(v_iso-iso);
		if(curDiff > maxDiff){
			maxV = curV;
			maxDiff = curDiff;
		}
	}
	anchorV = maxV;
	isAnchorVPositive = allCellTets.cells[cellId].VPro[anchorV] - iso > 0 ? true : false;
}

bool Arrangement::sufCell_isTriOriented(int cellId, int curTet, float iso, vector<float> &v0, vector<float> &v1, vector<float> &v2){
	int curV;
	float maxAbsDotRes = -1.0f;
	int anchorV = -1;
	int anchorVi = -1;
	vector<Eigen::Vector3f> pt_sufv(3);
	pt_sufv[0] = Eigen::Vector3f(v0[0],v0[1],v0[2]);
	pt_sufv[1] = Eigen::Vector3f(v1[0],v1[1],v1[2]);
	pt_sufv[2] = Eigen::Vector3f(v2[0],v2[1],v2[2]);
	Eigen::Vector3f sufNorm = (pt_sufv[1]-pt_sufv[0]).cross((pt_sufv[2]-pt_sufv[1]));
	Eigen::Vector3f curNorm;
	vector<float> dotRes(4);
	float squaredNorms;
	for(int i=0; i<4; ++i){
		curV = allCellTets.cells[cellId].cellTets[curTet].v[i];
		curNorm = Eigen::Vector3f(allCellTets.cells[cellId].cellVertices[curV].xyz[0],allCellTets.cells[cellId].cellVertices[curV].xyz[1],allCellTets.cells[cellId].cellVertices[curV].xyz[2])- pt_sufv[0];
		dotRes[i] = curNorm.dot(sufNorm);

		if(abs(dotRes[i]) > maxAbsDotRes){
			anchorV = curV;
			anchorVi = i;
			maxAbsDotRes = abs(dotRes[i]);
		}
	}
	bool isAnchorVPositive = allCellTets.cells[cellId].VPro[anchorV] - iso > 0 ? true : false;
	return (dotRes[anchorVi] > 0) != isAnchorVPositive;
}

int Arrangement::sufCell_MingTriMultPoly (
	// input
	vector<vector<vector<float> > > &inCurves,
	vector<vector<vector<float> > > &inNorms,
	bool useDelaunay,
	bool useMinSet,
	bool useNormal,
	float areaWeight,
	float edgeWeight,
	float dihedralWeight,
	float boundaryNormalWeight,
	// output
	int &_numofpoints, 
	int &_numoftilingtris, 
	float * _pPositions, 
	float * _pNormals, 
	int * _pFaceIndices,
	int cellId,
	int setId
	){

	try{	
		// init
		DMWT myDMWT (inCurves,inNorms, useDelaunay, useMinSet, useNormal);
		myDMWT.setWeights(areaWeight, edgeWeight, dihedralWeight, boundaryNormalWeight);

		// prepare datastructures
		myDMWT.buildList();
	
		// tiling
		myDMWT.tile();
	
		// save results
		myDMWT.saveResults(_numofpoints, _numoftilingtris, _pPositions, _pNormals, _pFaceIndices );
		myDMWT.saveTilingObj((t_savedir+"Triangulate_"+Utility::toStr(cellId)+"_"+Utility::toStr(setId)+".obj").c_str());

	}catch(int e){
		cout<<"DMWT: Error!! Exception Nr. "<<e<<endl;
		return 1; 
	}

	return 0;
}

int Arrangement::sufCell_MingTriMultPoly(
		// input
		vector<vector<vector<float> > > &inCurves,
		vector<vector<vector<float> > > &inNorms,
		bool useDelaunay,
		bool useMinSet,
		bool useNormal,
		float areaWeight,
		float edgeWeight,
		float dihedralWeight,
		float boundaryNormalWeight,
		// output
		vector<vector<int> > &sufF, 
		int cellId,
		int setId
	){
	try{	
		// init
		DMWT myDMWT (inCurves,inNorms, useDelaunay, useMinSet, useNormal);
		myDMWT.setWeights(areaWeight, edgeWeight, dihedralWeight, boundaryNormalWeight);

		// prepare datastructures
		myDMWT.buildList();
	
		// tiling
		myDMWT.tile();
	
		int nV = 0;
		for(size_t i=0; i<inCurves.size(); ++i){
			nV += inCurves[i].size();
		}
		// save results
		myDMWT.saveResults( sufF );
	}catch(int e){
		cout<<"DMWT: Error!! Exception Nr. "<<e<<endl;
		return 1; 
	}

	return 0;
}
