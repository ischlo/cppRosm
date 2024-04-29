# reviewing osm map features in an attempt to develop a straightforward way to work with them.
# One that would be performant and not result in data sets full of NAs. 


# The idea is to develop a 2 level hierarchy of keys. The first level would contain the 'main' ones.
# such as the 'highway' key for roads, or 'amenity' key for pois. 
# The specificity of this is that such a key is uniquely defines the osm element it is attached to. 
# The second level of keys is left for less important attributes, which generally serve less and 
# are not necessarily common in the data. 
# for example the 'addr:...' keys, or the sub keys relating to a road segments like 'service'.

# 1st version:
# the data schema becomes the following: 
# * 1st level keys define the data set and should be stored as a separate column variable.
# * 2nd level keys are grouped into a named list/vector and put in a column variable. 
# The structure for each osm element should the following: list('key1'='value1','key2'='value2'...)

# 2nd version:
# the data schema:
# 1st level key~value pairs are stored in 2 columns key, value.
# * 2nd level keys are grouped into a named list/vector and put in a column variable. 
# The structure for each osm element should the following: list('key1'='value1','key2'='value2'...)

# having good support functions to iterate into the lists of tags for each element will make life easy.

# Generally, 2nd order keys are dependent upon 1st level ones, for example :
# 1st level: leisure=pitch
# 2nd level: sport=athletics
# the second level adds information to the 1st level, but would not be there without the 1st one. 

# Best practice rules.

# another column variable containing a list of values is the nodes.

# This means that a separate data set needs to contain only node coordinates. 
# this way one can reconstruct the geometry of any feature by merging the elements of interest with the node data set. 
# this avoids storing geometries all the time, potentially speeding up things.

map_keys_1st <- c('aerialway'
                  ,'aeroway'
                  ,'barrier'
                  ,'boundary'
                  ,'building'
                  ,'craft'
                  ,'emergency'
                  ,'geological'
                  ,'healthcare'
                  ,'highway'
                  ,'historic'
                  ,'landuse'
                  ,'leisure'
                  ,'man_made'
                  ,'military'
                  ,'natural'
                  ,'office'
                  ,'place'
                  ,'power'
                  ,'public_transport'
                  ,'railway'
                  ,'shop'
                  ,'telecom'
                  ,'tourism'
                  ,'waterway'
                  )

# all the other keys are considered 2nd level