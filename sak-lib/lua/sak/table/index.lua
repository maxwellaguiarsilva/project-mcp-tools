#!/usr/bin/lua


local function find_method( self, key )
	local	class, value	=	table.get_class( self )
	while true do
		if is_function( value ) then return value end
		if not table.get_is_class( class ) then return nil end
		value	=	rawget( class, key )
		class	=	class ~= table and table.get_private( class ).super
	end
end

function table:__index( key )
	local	result	=	find_method( self, key )
	if not is_nil( result ) or not is_string( key ) then return result end
	return	safe_call( find_method( self, "get_" .. key ), self )
end

local	private_key_message		=	"private key: %s[ %s ]"
function table:__newindex( key, value )
	local	method		=	find_method( self, "set_" .. key )
	if is_function( method ) then return method( self, value ) end
	if is_function( find_method( self, "get_" .. key ) ) then return error( private_key_message:format( self.class.name, key ) ) end
	rawset( self, key, value )
end


