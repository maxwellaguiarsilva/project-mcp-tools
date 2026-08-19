#!/usr/bin/lua


unpack			=	table.unpack or unpack
table.unpack	=	table.unpack or unpack
table.push		=	table.insert
table.pop		=	table.remove
function table:find( item ) for key, value in pairs( self ) do if item == value then return key end end end
function table:globalize( ) for key, value in self:pairs( ) do _G[ key ] = value end end


stack:local_require{
	 "add"
	,"each"
	,"getters"
	,"index"
	,"pairs"
	,"set_class"
	,"tostring"
}


