#!/usr/bin/lua


function table:get_class( ) return safe_index( getmetatable( self ), "class" ) end
function table:get_is_class( ) return table.get_class( self ) == _G.class end
function table:get_metatable( ) return getmetatable( self ) end
function table:get_private( ) return safe_index( getmetatable( self ), "private" ) end
function table:get_memory_address( )
	local	temp	=	getmetatable( self )
	setmetatable( self, nil )
	local	result	=	tostring( self ):match( "0x[0-9a-fA-F]+" )
	setmetatable( self, temp )
	return	result
end

function table:get_by_type( )
	local	result	=	{ }
	for key, value in self:pairs( ) do result[ type( value ) ] = value end
	return	table( result )
end

function table:get_keys( )
	local	result	=	table( )
	for key, value in self:pairs( ) do result:insert( key ) end
	return	result
end

function table:get_values( )
	local	result	=	table( )
	for key, value in self:pairs( ) do result:insert( value ) end
	return	result
end

function table:get_unpacked_dict( )
	local	result	=	{ }
	for _, list in self:pairs( ) do result[ list[ 1 ] ] = list[ 2 ] end
	return	table( result )
end

function table:get_no_empty( )
	while self:find( "" ) do self:remove( self:find( "" ) ) end
	return	self
end


function table:get_first( ) return self[ 1 ] end
function table:get_last( ) return self[ #self ] end
function table:get_length( ) return #self end


