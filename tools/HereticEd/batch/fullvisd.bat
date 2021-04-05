@echo Full Vis
@echo .
bin_nt\qbsp3 %1
bin_nt\qvis3 %1
bin_nt\qrad3 -chop 256 %1
