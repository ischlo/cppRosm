#' Assert command line tool is installed
#'
#' Asserts that a tool is installed, throwing an error otherwise.
#' Shamelessly stolen from rosmium
#' @return Throws an error if the tool is not installed, invisibly returns `TRUE`
#'   otherwise.
#'
#' @keywords internal
#' @noRd
assert_tool_is_installed <- function(tool) {
  is_installed <- tryCatch(
    processx::run(tool, "--version"),
    error = function(cnd) cnd
  )
  
  if (inherits(is_installed, "error")) {
    
      cli::cli_abort(
        glue::glue("Could not find `{tool}` in system."
                   ,"Please make sure it has been installed and added to PATH."),
      )
  }
  
  return(invisible(TRUE))
}






