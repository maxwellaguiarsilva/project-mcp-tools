#!/usr/bin/lua


class	=	ensure_table( class )

function class:__call( ... )
	local	instance	=	( { ... } )[1]
	instance	=	( self == table and is_table( instance ) ) and instance or { }
	if self == table and instance.class == table then return instance end
	table.set_class( instance, self, ... )
	if self == class then
		local	this		=	instance.private
		local	args		=	{ }
		for key, value in pairs( { ... } ) do args[ type( value ) ] = value end
		this.super			=	args.table or table
		this.name			=	args.string or ( "[unnamed class %s]" ):format( instance:get_memory_address( ) )
	end
	return	instance
end


function class:set_default_getters( index )
	if is_table( index ) then for key, value in pairs( index ) do self:set_default_getters( value ) end ; return end
	self[ "get_" .. index ]	=	function( self ) return self.private[ index ] end
end

function class:set_default_setters( index )
	if is_table( index ) then for key, value in pairs( index ) do self:set_default_setters( value ) end ; return end
	self[ "set_" .. index ]	=	function( self, value ) self.private[ index ] = value; return value end
end

function class:get_name( ) return self.private.name end
function class:get_super( ) if self ~= table then return self.private.super end end
function class:__tostring( ) return self.name .. ( self.super and ( "::" .. tostring( self.super ) ) or "" ) end

local	set_class, get_private	=	table.set_class, table.get_private
set_class( class, class )
get_private( class ).super		=	table
get_private( class ).name		=	"class"
set_class( class, class )
set_class( table, class )
get_private( table ).name		=	"table"


