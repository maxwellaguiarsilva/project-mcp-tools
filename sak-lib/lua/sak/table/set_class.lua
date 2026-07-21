#!/usr/bin/lua


local	event_list	=	{
	 "add"	,"sub"	,"mul"	,"div"
	,"mod"	,"pow"	,"unm"	,"idiv"
	,"band"	,"bor"	,"bxor"	,"bnot"	,"shl"	,"shr"
	,"eq"	,"lt"	,"le"
	,"concat"	,"tostring"	,"len"
	,"call"
}
function table:set_class( class, ... )
	local	metatable		=	ensure_table( getmetatable( self ) )
	metatable.private		=	ensure_table( metatable.private )
	metatable.class			=	class
	metatable.__index		=	table.__index
	metatable.__newindex	=	table.__newindex
	setmetatable( self, metatable )
	for key, event_name in pairs( event_list ) do
		event_name	=	"__" .. event_name
		metatable[ event_name ]	=	metatable[ event_name ] or table.__index( self, event_name )
	end
	local	class_name	=	safe_index( table.get_private( class ), "name" )
	if table.get_is_class( class ) and class_name then safe_call( rawget( class, class_name ), self, ... ) end
	return	self
end




