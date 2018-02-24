@echo off 
::set exe=CycleGrouping_serial.exe
set exe=CycleGrouping_parallel.exe

set in_contour=./Data/ChickHeart/CTR/chick_sparse.contour
set in_volume=./Data/ChickHeart/Vol/chick.hdr
set in_volume_bbox=NULL
set in_genus=1
set out_dir=./Data/ChickHeart/_RESULT/Sparse/wVol/

set tet_limit=10
set beta=50

set smooth_bef_loop=3000
set smooth_n_loop=2
set smooth_in_loop=300

%exe% %in_contour% %in_volume% %in_volume_bbox% %out_dir% %tet_limit% %beta% %smooth_bef_loop% %smooth_n_loop% %smooth_in_loop% %in_genus% 
echo =================== DONE ===================
pause