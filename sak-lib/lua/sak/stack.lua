#!/usr/bin/lua


local	meta_table		=	{ __index = table }
function meta_table:__tostring( ) return self:concat( "." ) end
local	stack		=	setmetatable( { }, meta_table )



function stack:require( library, no_stack )
	local	result		=	{ }
	if type( library ) == "table" then
		for key, value in pairs( library ) do result[ #result ] = self:require( value, no_stack ) end
		return	result
	end
	if no_stack then return require( ( "%s.%s" ):format( self, library ) ) end
	self:insert( library )
	result		=	require( tostring( self ) )
	self:remove( )
	return	result
end


function stack:local_require( library ) self:require( library, true ) end

_G.stack	=	stack

return	stack


