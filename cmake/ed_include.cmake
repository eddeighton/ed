
cmake_minimum_required( VERSION 3.1...3.16 )

function( link_ed targetname )
	target_link_libraries( ${targetname} edlib )
endfunction( link_ed )
