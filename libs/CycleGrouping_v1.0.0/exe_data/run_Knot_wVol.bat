@echo off 
::set exe=CycleGrouping_serial.exe
set exe=CycleGrouping_parallel.exe

set in_contour=./Data/Knot/CTR/knot.contour
set in_volume=./Data/Knot/Vol/knot.mrc
set in_volume_bbox=./Data/Knot/BBOX/knot.bbox
set in_genus=1
set out_dir=./Data/Knot/_RESULT/wVol/

set tet_limit=5
set beta=80

set smooth_bef_loop=800
set smooth_n_loop=10
set smooth_in_loop=100

%exe% %in_contour% %in_volume% %in_volume_bbox% %out_dir% %tet_limit% %beta% %smooth_bef_loop% %smooth_n_loop% %smooth_in_loop% %in_genus% 
echo =================== DONE ===================
pause