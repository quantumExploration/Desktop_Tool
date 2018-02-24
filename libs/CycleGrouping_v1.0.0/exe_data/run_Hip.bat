@echo off 
::set exe=CycleGrouping_serial.exe
set exe=CycleGrouping_parallel.exe

set in_contour=./Data/Hip/CTR/hip.contour
set in_volume=NULL
set in_volume_bbox=NULL
set in_genus=2
set out_dir=./Data/Hip/_RESULT/

set tet_limit=10
set beta=0

set smooth_bef_loop=500
set smooth_n_loop=2
set smooth_in_loop=200

%exe% %in_contour% %in_volume% %in_volume_bbox% %out_dir% %tet_limit% %beta% %smooth_bef_loop% %smooth_n_loop% %smooth_in_loop% %in_genus% 
echo =================== DONE ===================
pause