#!/usr/bin/lua


function table:get_getter_keys( )
	local	keys	=	{ }
	local	list	=	table( )
	local	class	=	self.class
	local	key		=	""
	while table.get_is_class( class ) do
		for getter_method_name in class:pairs( ) do
			if getter_method_name:starts_with( "get_" ) then
				key	=	getter_method_name:sub( 5 )
				keys[ key ]	=	getter_method_name
				list:insert( key )
			end
		end
		class	=	class.super
	end
	return	list, keys
end


function table:__next_getter( index )
	local	keys
	if not index then keys, self.private.getter_keys_cache = self:get_getter_keys( ) end
	keys	=	self.private.getter_keys_cache
	local	key	=	next( keys, index )
	return	key, self[ key ]
end

function table:getter_pairs( ) return table.__next_getter, self end

table.pairs		=	pairs
table.__pairs	=	pairs


