/*
Copyright (c) 2000 Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied 
warranty. In no event will the authors be held liable for any 
damages arising from the use of this software.

Permission is granted to anyone to use this software for any 
purpose, including commercial applications, and to alter it and 
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must 
not claim that you wrote the original software. If you use this 
software in a product, an acknowledgment in the product documentation 
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and 
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source 
distribution.
*/


#include "tinyxml.h"
#include <string.h>
#include <ctype.h>

const char* TiXmlBase::SkipWhiteSpace( const char* p )
{
	while ( p && *p && 
	        ( isspace( *p ) || *p == '\n' || *p == '\r' ) )
		p++;
	return p;
}

const char* TiXmlBase::ReadName( const char* p, std::string* name )
{
	*name = "";
	const char* start = p;

	// Names start with letters or underscores.
	// After that, they can be letters, underscores, numbers,
	// hyphens, or colons. (Colons are valid ony for namespaces,
	// but tinyxml can't tell namespaces from names.)
	if ( p && ( isalpha( *p ) || *p == '_' ) )
	{
		p++;
		while( p && *p && 
			   (   isalnum( *p ) 
			     || *p == '_'
				 || *p == '-'
				 || *p == ':' ) )
		{
			p++;
		}
		name->append( start, p - start );
		return p;
	}
	return 0;
}


const char* TiXmlBase::ReadText(	const char* p, 
									std::string* text, 
									bool trimWhiteSpace, 
									const char* endTag, 
									bool caseInsensitive )
{
    *text = "";
	if ( !trimWhiteSpace			// certain tags always keep whitespace
		 /*|| !condenseWhiteSpace*/ )	// if true, whitespace is always kept
	{
		// Keep all the white space.
		while (	   p && *p
				&& strncmp( p, endTag, strlen(endTag) ) != 0
			  )
		{
			char c = *(p++);
            (* text) += c;
		}
	}
	else
	{
		bool whitespace = false;

		// Remove leading white space:
		p = SkipWhiteSpace( p );
		while (	   p && *p
				&& strncmp( p, endTag, strlen(endTag) ) != 0 )
		{
			if ( *p == '\r' || *p == '\n' )
			{
				whitespace = true;
				++p;
			}
			else if ( isspace( *p ) )
			{
				whitespace = true;
				++p;
			}
			else
			{
				// If we've found whitespace, add it before the
				// new character. Any whitespace just becomes a space.
				if ( whitespace )
				{
               (* text) += ' ';
					whitespace = false;
				}
				char c = *(p++);
            (* text) += c;
			}
		}
	}
	return p + strlen( endTag );
}


const char* TiXmlDocument::Parse( const char* start )
{
	// Parse away, at the document level. Since a document
	// contains nothing but other tags, most of what happens
	// here is skipping white space.
	
	const char* p = start;

 	p = SkipWhiteSpace( p );
	if ( !p || !*p )
	{
		error = true;
		errorDesc = "Document empty.";
	}
	
	while ( p && *p )
	{	
		if ( *p != '<' )
		{
			error = true;
			errorDesc = "The '<' symbol that starts a tag was not found.";
			break;
		}
		else
		{
			TiXmlNode* node = IdentifyAndParse( &p );
			if ( node )
			{
				LinkEndChild( node );
			}				
		}
		p = SkipWhiteSpace( p );
	}
	return 0;	// Return null is fine for a document: once it is read, the parsing is over.
}


TiXmlNode* TiXmlNode::IdentifyAndParse( const char** where )
{
	const char* p = *where;
	TiXmlNode* returnNode = 0;
	assert( *p == '<' );
	TiXmlDocument* doc = GetDocument();

	p = SkipWhiteSpace( p+1 );

	// What is this thing? 
	// - Elements start with a letter or underscore, but xml is reserved.
	// - Comments: <!--
	// - Everthing else is unknown to tinyxml.
	//
	if ( 	   tolower( *(p+0) ) == '?'
			&& tolower( *(p+1) ) == 'x' 
			&& tolower( *(p+2) ) == 'm'
			&& tolower( *(p+3) ) == 'l' )
	{
		#ifdef DEBUG_PARSER
			printf( "XML parsing Declaration\n" );
		#endif
		returnNode = new TiXmlDeclaration();
	}
	else if ( isalpha( *p ) || *p == '_' )
	{
		#ifdef DEBUG_PARSER
			printf( "XML parsing Element\n" );
		#endif
		returnNode = new TiXmlElement( "" );
	}
	else if (    *(p+0) == '!'
			  && *(p+1) == '-'
			  && *(p+2) == '-' )
	{
		#ifdef DEBUG_PARSER
			printf( "XML parsing Comment\n" );
		#endif
		returnNode = new TiXmlComment();
	}
	else if ( strncmp(p, "![CDATA[", 8) == 0 )
	{
		TiXmlNode* cdataNode = new TiXmlCData();
		if ( !cdataNode )
		{
			if ( doc ) doc->SetError( TIXML_ERROR_OUT_OF_MEMORY );
			return 0;
		}

		returnNode = cdataNode;
	}
	else
	{
		#ifdef DEBUG_PARSER
			printf( "XML parsing Comment\n" );
		#endif
		returnNode = new TiXmlUnknown();
	}

	if ( returnNode )
	{
		// Set the parent, so it can report errors
		returnNode->parent = this;
		p = returnNode->Parse( p );
	}
	else
	{
		if ( doc )
			doc->SetError( TIXML_ERROR_OUT_OF_MEMORY );
		p = 0;
	}
	*where = p;
	return returnNode;
}


const char* TiXmlElement::Parse( const char* p )
{
	TiXmlDocument* document = GetDocument();
	p = SkipWhiteSpace( p );
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_PARSING_ELEMENT );
		return 0;
	}

	// Read the name.
	p = ReadName( p, &value );
	if ( !p )
	{
		if ( document )	document->SetError( TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME );
		return 0;
	}

	std::string endTag = "</";
	endTag += value;
	endTag += ">";

	// Check for and read attributes. Also look for an empty
	// tag or an end tag.
	while ( p && *p )
	{
		p = SkipWhiteSpace( p );
		if ( !p || !*p )
		{
			if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
			return 0;
		}
		if ( *p == '/' )
		{
			// Empty tag.
			if ( *(p+1) != '>' )
			{
				if ( document ) document->SetError( TIXML_ERROR_PARSING_EMPTY );		
				return 0;
			}
			return p+2;
		}
		else if ( *p == '>' )
		{
			// Done with attributes (if there were any.)
			// Read the value -- which can include other
			// elements -- read the end tag, and return.
			p = ReadValue( p+1 );		// Note this is an Element method, and will set the error if one happens.
			if ( !p )
				return 0;

			// We should find the end tag now
			std::string buf( p, endTag.size() );
			if ( endTag == buf )
			{
				return p+endTag.size();
			}
			else
			{
				if ( document ) document->SetError( TIXML_ERROR_READING_END_TAG );
				return 0;
			}
		}
		else
		{
			// Try to read an element:
			TiXmlAttribute attrib;
			attrib.SetDocument( document );
			p = attrib.Parse( p );

			if ( p )
			{
				SetAttribute( attrib.Name(), attrib.Value() );
			}
		}
	}
	return 0;
}


const char* TiXmlElement::ReadValue( const char* p )
{
	TiXmlDocument* document = GetDocument();

	// Read in text and elements in any order.
	p = SkipWhiteSpace( p );
	while ( p && *p )
	{
		const char* start = p;
		while ( *p && *p != '<' )
			p++;

		if ( !*p )
		{
			if ( document ) document->SetError( TIXML_ERROR_READING_ELEMENT_VALUE );
			return 0;
		}
		if ( p != start )
		{
			// Take what we have, make a text element.
			TiXmlText* text = new TiXmlText();

			if ( !text )
			{
				if ( document ) document->SetError( TIXML_ERROR_OUT_OF_MEMORY );
				return 0;
			}
			text->Parse( start );
			if ( !text->Blank() )
				LinkEndChild( text );
			else
				delete text;
		} 
		else 
		{
			// We hit a '<'
			// Have we hit a new element or an end tag?
			if ( *(p+1) == '/' )
			{
				return p;	// end tag
			}
			else
			{
// 				TiXmlElement* element = new TiXmlElement( "" );
// 
// 				if ( element )
// 				{
// 					p = element->Parse( p+1 );
// 					if ( p )
// 						LinkEndChild( element );
// 				}
// 				else
// 				{
// 					if ( document ) document->SetError( ERROR_OUT_OF_MEMORY );
// 					return 0;
// 				}
				TiXmlNode* node = IdentifyAndParse( &p );
				if ( node )
				{
					LinkEndChild( node );
				}				
				else
				{
					return 0;
				}
			}
		}
	}
	return 0;
}


const char* TiXmlUnknown::Parse( const char* p )
{
	const char* end = strchr( p, '>' );
	if ( !end )
	{
		TiXmlDocument* document = GetDocument();
		if ( document )
			document->SetError( TIXML_ERROR_PARSING_UNKNOWN );
		return 0;
	}
	else
	{
		value = std::string( p, end-p );
// 		value.resize( end - p );
		return end + 1;			// return just past the '>'
	}
}


const char* TiXmlComment::Parse( const char* p )
{
	assert( *p == '!' && *(p+1) == '-' && *(p+2) == '-' );

	// Find the end, copy the parts between to the value of
	// this object, and return.
	const char* start = p+3;
	const char* end = strstr( p, "-->" );
	if ( !end )
	{
		TiXmlDocument* document = GetDocument();
		if ( document )
			document->SetError( TIXML_ERROR_PARSING_COMMENT );
		return 0;
	}
	else
	{
		// Assemble the comment, removing the white space.
		bool whiteSpace = false;

		const char* q;
		for( q=start; q<end; q++ )
		{
			if ( isspace( *q ) )
			{
				if ( !whiteSpace )
				{
					value += ' ';
					whiteSpace = true;
				}
			}
			else
			{
				value += *q;
				whiteSpace = false;
			}
		}				
// 		value = std::string( start, end-start );
		return end + 3;			// return just past the '>'
	}
}


const char* TiXmlAttribute::Parse( const char* p )
{
	// Read the name, the '=' and the value.
	p = ReadName( p, &name );
	if ( !p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
		return 0;
	}
	p = SkipWhiteSpace( p );
	if ( !p || *p != '=' )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
		return 0;
	}

	p = SkipWhiteSpace( p+1 );
	if ( !p || !*p )
	{
		if ( document ) document->SetError( TIXML_ERROR_READING_ATTRIBUTES );
		return 0;
	}
	
	const char* end = 0;
	const char* start = p+1;
	const char* past = 0;

	if ( *p == '\'' )
	{
		end = strchr( start, '\'' );
		past = end+1;
	}
	else if ( *p == '"' )
	{
		end = strchr( start, '"' );
		past = end+1;
	}
	else
	{
		// All attribute values should be in single or double quotes.
		// But this is such a common error that the parser will try
		// its best, even without them.
		start--;
		for ( end = start; *end; end++ )
		{
			if ( isspace( *end ) || *end == '/' || *end == '>' )
				break;
		}
		past = end;
	}
	value = std::string( start, end-start );
	return past;
}


const char* TiXmlText::Parse( const char* p )
{
	value = "";
	bool ignoreWhite = true;

	const char* end = "<";
	p = ReadText( p, &value, ignoreWhite, end, false );

	if ( p )
		return p-1;     // don't truncate the '<'
	return 0;

#if 0
	// Remove leading white space:
	p = SkipWhiteSpace( p );
	while ( *p && *p != '<' )
	{
		if ( *p == '\r' || *p == '\n' )
		{
			whitespace = true;
		}
		else if ( isspace( *p ) )
		{
			whitespace = true;
		}
		else
		{
			// If we've found whitespace, add it before the
			// new character. Any whitespace just becomes a space.
			if ( whitespace )
			{
				value += ' ';
				whitespace = false;
			}
			value += *p;
		}
		p++;
	}
	// Keep white space before the '<' 
	if ( whitespace )
		value += ' ';

	return p;
#endif
}


const char* TiXmlCData::Parse( const char* p )
{
	value = "";

	bool ignoreWhite = false;
	p += 8;
	const char* end = "]]>";
	p = ReadText( p, &value, ignoreWhite, end, false );
	if ( p )
		return p;
	return 0;
}


const char* TiXmlDeclaration::Parse( const char* p )
{
	// Find the beginning, find the end, and look for
	// the stuff in-between.
	const char* start = p+4;
	const char* end  = strstr( start, "?>" );

	// Be nice to the user:
	if ( !end )
	{
		end = strstr( start, ">" );
		end++;
	}
	else
	{
		end += 2;
	}

	if ( !end )
	{
		TiXmlDocument* document = GetDocument();
		if ( document )
			document->SetError( TIXML_ERROR_PARSING_DECLARATION );
		return 0;
	}
	else
	{
		const char* p;
		
		p = strstr( start, "version" );
		if ( p && p < end )
		{
			TiXmlAttribute attrib;
			attrib.Parse( p );		
			version = attrib.Value();
		}

		p = strstr( start, "encoding" );
		if ( p && p < end )
		{
			TiXmlAttribute attrib;
			attrib.Parse( p );		
			encoding = attrib.Value();
		}

		p = strstr( start, "standalone" );
		if ( p && p < end )
		{
			TiXmlAttribute attrib;
			attrib.Parse( p );		
			standalone = attrib.Value();
		}
	}
	return end;
}

bool TiXmlText::Blank()
{
	for ( unsigned i=0; i<value.size(); i++ )
		if ( !isspace( value[i] ) )
			return false;
	return true;
}

