import arcpy

sfc_fullpath = "C:/gdb/my_python_gdb.gdb/my_python_featureclass"
sfield_name = "my_python_field"
sfield_type = "LONG"

arcpy.AddField_management(sfc_fullpath, sfield_name, sfield_type)

input ("Field added successfully, press any key to continue..")
