#!/usr/bin/lua


local	indentation_text		=	"    "
local	line_feed			=	"\n"
local	item_template		=	"%s%s: %s"
local	object_template		=	"%s[ %s ]" .. line_feed
local	indented_line_feed	=	line_feed .. indentation_text

local function format_indentation( key, value )
	if is_table( value ) then value = tostring( value ):gsub( line_feed, indented_line_feed ) end
	return	item_template:format( indentation_text, tostring( key ), tostring( value ) )
end


function table:__tostring( )
	return	object_template:format( self.class.name ,self.memory_address ) .. ( self:each_pair( format_indentation ):concat( line_feed ) )
end


