#!/usr/bin/lua


function table:update( other ) for key, value in pairs( other ) do self[ key ] = value end; return self end
function table:copy( other ) table.update( ensure_table( other ), self ); return self end


function table:__add( other )
	if not is_table( other ) then
		table.insert( self, other )
	else
		for key, value in pairs( other ) do self[ is_number( key ) and ( #self + key ) or key ] = value end
	end
	return	self
end


function table:__concat( other )
	if is_string( other ) then return table.concat( self, other ) end
	local	result	=	table.update( { }, self )
	return	table.__add( result, other )
end




