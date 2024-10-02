#'@title Extracting road graphs from OSM
#'@name extract_graph
#'@description
#'This function helps extract a road network graph in a simple and light weight format.
#'In the background, it uses the power of the osmium library in C++, and copies the data in graph format locally as 2 .csv 
#'files, one with nodes and their coordinates, with with segments and optional values. 
#'
#'@param filename a local osm extract file with .osm or .osm.pbf extension.
#'
# local_osm_filename <- system.file("extdata",'map.osm', package = "cppRosm")
#
# cppRosm::extract_graph(local_osm_filename)
#'
#'@return this function returns the value 1 to the R environment when it's completed.
#' It writes out 2 files containing nodes and segments. The files are written in the current directory and are called:
#' `nodes.csv`,`road_segments.csv`.
#'@export
extract_graph <- function(filename){
  
  # assert_tool_is_installed('osmium')
  
  f_names <- c('nodes.csv','road_segments.csv')
  
  if(any(sapply(f_names,file.exists))) stop('Filenames `nodes.csv` or `road_segments.csv` exist, please rename or remove.')
  
  cat('Files will be written to: ',getwd(),'\n')
  
  # graph_data <- cpp_extract_graph(filename)
  # graph_data <- lapply(graph_data,data.table::as.data.table)
  # graph_data$nodes <- graph_data$nodes[!duplicated(graph_data$nodes$id),]
  # return(graph_data)
  
  cpp_extract_graph(filename)
}
