#'@title Extracting road graphs from OSM
#'@name extract_graph
#'@description
#'This function helps extract a road network graph in a simple and light weight format.
#'In the background, it uses the power of the osmium library in C++, and copies the data iin graph format locally as 2 .csv 
#'files, one with nodes and their coordinates, with with segments and optional values. 
#'
#'@param filename a local osm extract file with .osm or .osm.pbf extension.
#'
#'
#'
#'@export
extract_graph <- function(filename){
  
  assert_osmium_is_installed()
  
  f_names <- c('nodes.csv','road_segments.csv')
  
  if(any(sapply(f_names,file.exists))) stop('Filenames `nodes.csv` or `road_segments.csv` exist, please rename or remove.')
  
  cat('Files will be written to: ',getwd(),'\n')
  
  cpp_extract_graph(filename)
}




