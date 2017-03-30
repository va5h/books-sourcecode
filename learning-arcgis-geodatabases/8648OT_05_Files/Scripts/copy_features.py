import arcpy

sfc_source = "c:/gdb/my_python_gdb.gdb/my_python_featureclass"

sfc_dest =  "c:/gdb/my_python_gdb.gdb/my_python_featureclass_copy"

arcpy.CopyFeatures_management (sfc_source, sfc_dest)

input ("Feature class copied successfully, press any key to continue...")
