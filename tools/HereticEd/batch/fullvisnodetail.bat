@echo Full Vis No Detail
@echo .
qbsp3 -nodetail %1
qvis3 %1
qrad3 -extra %1
