#'@title Extracting road graphs from OSM
#'@name extract_graph
#'@description
#'This function helps extract a road network graph in a simple and light weight format.
#'In the background, it uses the power of the osmium library in C++, and copies the data in graph format locally as 2 .csv 
#'files, one with nodes and their coordinates, with with segments and optional values. 
#'
#'@param filename a local osm extract file with .osm or .osm.pbf extension.
#'@param out a local osm extract file with .osm or .osm.pbf extension.
#'
# local_osm_filename <- system.file("extdata",'monaco-latest.osm.pbf', package = "cppRosm")
#
# cppRosm::extract_graph(local_osm_filename)
#'
#'@return this function returns the value 1 to the R environment when it's completed.
#' It writes out 2 files containing nodes and segments. The files are written in the current directory and are called:
#' `nodes.csv`,`road_segments.csv`.
#'@export
extract_graph <- function(filename,out = NULL){
  
  # assert_tool_is_installed('osmium')
  out <- ifelse(is.null(out),"",out)
  
  f_names <- paste(out,c('nodes.csv','road_segments.csv'))
  
  if(any(sapply(f_names,file.exists))) stop('Filenames `nodes.csv` or `road_segments.csv` exist, please rename or remove.')
  
  cli::cli_alert_info('Files will be written to: ',getwd(),'\n')
  
  cpp_extract_graph(filename,out)
}
