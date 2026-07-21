#!/usr/bin/lua


function string:starts_with( text ) return self:sub( 1, #text ) == text end
function string:ends_with( text ) return self:sub( -#text ) == text end


function string:convert( )
	if self == "" then return nil end
	if self == "true" then return true end
	if self == "false" then return false end
	local	number	=	tonumber( self )
	if is_number( number ) then return number end
	return	self
end


function string:enclose( before, after )
	before	=	is_string( before ) and before or "\""
	after	=	is_string( after ) and after or before
	return	before .. self .. after
end
string.quote	=	string.enclose


function string:split( delimiter )
	local	first_match	=	( "(.-)%s" ):format( delimiter )
	local	last_match	=	( ".*%s(.-)$" ):format( delimiter )
	local	result	=	table( )
	for match in self:gmatch( first_match ) do result:insert( match ) end
	if self:find( last_match ) then result:insert( self:match( last_match ) ) end
	if #result == 0 then result:insert( self ) end
	return	result
end

function string:lines( crlf ) return self:split( crlf or "[\r\n]+" ).no_empty end
function string:words( crlf ) return self:split( crlf or "[\r\n \\.]+" ).no_empty end
function string:csv( delimiter, crlf ) return self:lines( crlf ):apply( string.split, delimiter or "," ) end
function string:tsv( delimiter, crlf ) return self:csv( delimiter or "[\t]+", crlf ) end



function string:read_all( type )
	local	opener	=	( { file = io.open, process = io.popen } )[ type ] or error( "read_all: invalid type" )
	local	output	=	opener( self ) or error( ( [[real_all: error opening %s "%s"]] ):format( type, self ) )
	local	result	=	output:read( "*all" )
	output:close( )
	return	result
end

local	list_path_command	=	(	package.config:sub( 1, 1 ) == "/"	--	linux/windows
	and	[[find "%s" -type f]]
	or	[[pwsh -Command "gci -Recurse ""%s"" -File | select -ExpandProperty FullName"]]
)
function string:path_list( ) return list_path_command:format( self ):process_open( ):lines( ) end
function string:path_content( ) return self:path_list( ):map( string.file_content ) end
--	function string:process_open( ) return self:read_all( "process" ) end
function string:process_open( )
	if is_table( vim ) then return vim.fn.system( self ) end
	return	self:read_all( "process" ) 
end
function string:file_content( ) return self:read_all( "file" ) end
string.text_file	=	string.file_content


function string:unescape( )
	return self
		:gsub( [[\n]]	,"\n" )
		:gsub( [[\t]]	,"\t" )
		:gsub( [[\r]]	,"\r" )
		:gsub( [[\"]]	,"\"" )
		:gsub( [[\']]	,"\'" )
		:gsub( [[\\]]	,"\\" )
end


return	string


