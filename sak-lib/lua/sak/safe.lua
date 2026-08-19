#!/usr/bin/lua


local	type_list	=	{
	 ["string"]		=	function( ) return ""		end
	,["table"]		=	function( ) return { }		end
	,["boolean"]	=	function( ) return false	end
	,["number"]		=	function( ) return 0		end
	,["function"]	=	function( ) return function( ) end end
}

for type_name, type_default in pairs( type_list ) do
	local	type_check		=	function( self ) return type( self ) == type_name end
	local	type_coalesce	=	function( ... )
		for key, value in pairs( { ... } ) do if type_check( value ) then return value end end
	end
	_G[ "is_" .. type_name ]		=	type_check
	_G[ "coalesce_" .. type_name ]	=	type_coalesce
	_G[ "ensure_" .. type_name ]	=	function( self, alternative ) return type_coalesce( self, alternative, type_default( ) ) end
end

function is_nil( self ) return self == nil end
function is_callable( self ) return is_function( self ) or ( is_table( self ) and is_function( getmetatable( self ).__call ) ) end
function safe_call( action, ... ) if is_callable( action ) then return action( ... ) end end
function safe_index( self, index ) if is_table( self ) then return self[ index ] end end
function safe_table_call( self, index, ... ) return safe_call( safe_index( self, index ), ... ) end
function coalesce( ... ) for key, value in pairs( { ... } ) do if not is_nil( value ) then return value end end end

function safe_caller( action, callback )
	assert( is_callable( action ), "safe_caller: action must be a callable!" )
	return is_callable( callback )
		and	( function( ... ) return xpcall( action, callback, ... ) end )
		or	( function( ... ) return pcall( action, ... ) end )
end




