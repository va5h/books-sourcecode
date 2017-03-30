import arcpy
import os
import datetime

sgdb_path = "c:/gdb/web"

sgdb_name = "Web_Bestaurants.gdb"

todaydate = str(datetime.date.today().year) + str(datetime.date.today().month)  + str(datetime.date.today().day)

os.rename (sgdb_path + "/" + sgdb_name , sgdb_path + "/" + "Web_Bestaurants" + todaydate + ".gdb" )

arcpy.CreateFileGDB_management(sgdb_path, sgdb_name)

sfc_source = "c:/gdb/Bestaurants_new.gdb/Food_and_Drinks"

sfc_dest = sgdb_path + "/" + sgdb_name + "/Restaurants"

arcpy.CopyFeatures_management (sfc_source, sfc_dest)

sfield_rating = "RATING"

sfield_desc = "DESCRIPTION"

arcpy.DeleteField_management(sfc_dest, sfield_rating)

arcpy.DeleteField_management(sfc_dest, sfield_desc)


input ("Web Bestaurants geodatabase created successfully, press any key to continue.")

