#!/usr/bin/lua


--	each executes the function and inserts the results into a new list
--	apply executes the function and inserts the results into the same dictionary, keeping the original index
--	map executes the function and inserts the results into a new dictionary, using the value as the index for the result
local	closure	=	{ }
function closure:each( action, key, value, ... ) return action( value, ... ) end
function closure:each_list( action, key, value, ... ) return action( table.unpack( value ) ) end
function closure:each_pair( action, key, value, ... ) return action( key, value, ... ) end
function closure:each_list_pair( action, key, value, ... ) return action( key, table.unpack( value ) ) end
function closure:each_self_pair( action, key, value, ... ) return action( self, key, value, ... ) end
function closure:each_self_list_pair( action, key, value, ... ) return action( self, key, table.unpack( value ) ) end


local	suffix=""
for name, closure_action in pairs( closure ) do
	suffix=name:sub( 5 )
	table[ "each" .. suffix ]		=	function( self, action, ... )
		local	result	=	table( )
		for key, value in pairs( self ) do table.insert( result, closure_action( self, action, key, value, ... ) or nil ) end
		return	result
	end
	table[ "each_if" .. suffix ]	=	function( self, action, ... )
		local	result	=	table( )
		for key, value in pairs( self ) do table.insert( result, closure_action( self, action, key, value, ... ) and value or nil ) end
		return	result
	end
	table[ "each_if_not" .. suffix ]=	function( self, action, ... )
		local	result	=	table( )
		for key, value in pairs( self ) do table.insert( result, ( not closure_action( self, action, key, value, ... ) ) and value or nil ) end
		return	result
	end
	table[ "apply" .. suffix ]		=	function( self, action, ... )
		for key, value in pairs( self ) do self[ key ] = closure_action( self, action, key, value, ... ) end
		return	self
	end
	table[ "map" .. suffix ]		=	function( self, action, ... )
		local	result	=	table( )
		for key, value in pairs( self ) do result[ value ]	= closure_action( self, action, key, value, ... ) end
		return	result
	end
end



