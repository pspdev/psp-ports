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


#ifndef TINYXML_INCLUDED
#define TINYXML_INCLUDED

#include <string>
#include <stdio.h>
#include <assert.h>

class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;


/** TiXmlBase is a base class for every class in TinyXml.
	It does little except to establist that TinyXml classes
	can be printed and provide some utility functions.

	In XML, the document and elements can contain
	other elements and other types of nodes.

	@verbatim
	A Document can contain:	Element	(container or leaf)
							Comment (leaf)
							Unknown (leaf)
							Declaration( leaf )

	An Element can contain:	Element (container or leaf)
							Text	(leaf)
							Attributes (not on tree)
							Comment (leaf)
							Unknown (leaf)

	A Decleration contains: Attributes (not on tree)
	@endverbatim
*/
class TiXmlBase
{
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;
 
  public:
	TiXmlBase()								{}	
	virtual ~TiXmlBase()					{}
	
	/*	All TinyXml classes can print themselves to a filestream.
	*/
	virtual void Print( FILE* fp, int depth )	= 0;

  protected:
	/*	General parsing helper method. Takes a pointer in,
		skips all the white space it finds, and returns a pointer
		to the first non-whitespace data.
	*/
	static const char* SkipWhiteSpace( const char* p );

	/*	Reads an XML name into the string provided. Returns
		a pointer just past the last character of the name, 
		or 0 if the function has an error.
	*/
	static const char* ReadName( const char* p, std::string* name );

	/*	Reads text. Returns a pointer past the given end tag.
		Wickedly complex options, but it keeps the (sensitive)
		code in one place.
	*/
	static const char* ReadText( const char* in,		// where to start
								 std::string* text,      // the string read
								 bool ignoreWhiteSpace,	// whether to keep the white space
								 const char* endTag,	// what ends this text
								 bool ignoreCase );		// whether to ignore case in the end tag

	enum
	{
		TIXML_NO_ERROR = 0,
		TIXML_ERROR_OPENING_FILE,
		TIXML_ERROR_OUT_OF_MEMORY,
		TIXML_ERROR_PARSING_ELEMENT,
		TIXML_ERROR_FAILED_TO_READ_ELEMENT_NAME,
		TIXML_ERROR_READING_ELEMENT_VALUE,
		TIXML_ERROR_READING_ATTRIBUTES,
		TIXML_ERROR_PARSING_EMPTY,
		TIXML_ERROR_READING_END_TAG,
		TIXML_ERROR_PARSING_UNKNOWN,
		TIXML_ERROR_PARSING_COMMENT,
		TIXML_ERROR_PARSING_DECLARATION,

		TIXML_ERROR_STRING_COUNT
	};
	static const char* errorString[ TIXML_ERROR_STRING_COUNT ];
};


/** The parent class for everything in the Document Object Model.
	(Except for attributes, which are contained in elements.)
	Nodes have siblings, a parent, and children. A node can be
	in a document, or stand on its own. The type of a TyXmlNode
	can be queried, and it can be cast to its more defined type.
*/
class TiXmlNode : public TiXmlBase
{
  public:
	/** The types of XML nodes supported by TinyXml. (All the
		unsupported types are picked up by UNKNOWN.)
	*/
	enum NodeType 
	{
		DOCUMENT, ELEMENT, COMMENT, UNKNOWN, TEXT, DECLARATION, TYPECOUNT
	};

	virtual ~TiXmlNode();

	/** The meaning of 'value' changes for the specific type of
		TiXmlNode.
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim

		The subclasses will wrap this function.
	*/
	const std::string& Value()	const			{ return value; }

	/** Changes the value of the node. Defined as:
		@verbatim
		Document:	filename of the xml file
		Element:	name of the element
		Comment:	the comment text
		Unknown:	the tag contents
		Text:		the text string
		@endverbatim
	*/
	void SetValue( const std::string& _value )		{ value = _value; }

	/// Delete all the children of this node. Does not affect 'this'.
	void Clear();

	/// One step up the DOM.
	TiXmlNode* Parent() const					{ return parent; }

	TiXmlNode* FirstChild()	const	{ return firstChild; }		///< The first child of this node. Will be null if there are no children.
	TiXmlNode* FirstChild( const std::string& value ) const;	///< The first child of this node with the matching 'value'. Will be null if none found.
	
	TiXmlNode* LastChild() const	{ return lastChild; }		/// The last child of this node. Will be null if there are no children.
	TiXmlNode* LastChild( const std::string& value ) const;		/// The last child of this node matching 'value'. Will be null if there are no children.

	/** An alternate way to walk the children of a node.
		One way to iterate over nodes is:
		@verbatim
			for( child = parent->FirstChild(); child; child = child->NextSibling() )
		@endverbatim

		IterateChildren does the same thing with the syntax:
		@verbatim
			child = 0;
			while( child = parent->IterateChildren( child ) )
		@endverbatim

		IterateChildren takes the previous child as input and finds
		the next one. If the previous child is null, it returns the
		first. IterateChildren will return null when done.
	*/
	TiXmlNode* IterateChildren( TiXmlNode* previous );

	/// This flavor of IterateChildren searches for children with a particular 'value'
	TiXmlNode* IterateChildren( const std::string& value, TiXmlNode* previous );
		
	/** Add a new node related to this. Adds a child past the LastChild.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertEndChild( const TiXmlNode& addThis );					

	/** Add a new node related to this. Adds a child before the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertBeforeChild( TiXmlNode* beforeThis, const TiXmlNode& addThis );

	/** Add a new node related to this. Adds a child after the specified child.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* InsertAfterChild(  TiXmlNode* afterThis, const TiXmlNode& addThis );
	
	/** Replace a child of this node.
		Returns a pointer to the new object or NULL if an error occured.
	*/
	TiXmlNode* ReplaceChild( TiXmlNode* replaceThis, const TiXmlNode& withThis );
	
	/// Delete a child of this node.
	bool RemoveChild( TiXmlNode* removeThis );

	/// Navigate to a sibling node.
	TiXmlNode* PreviousSibling() const			{ return prev; }

	/// Navigate to a sibling node.
	TiXmlNode* PreviousSibling( const std::string& ) const;
	
	/// Navigate to a sibling node.
	TiXmlNode* NextSibling() const				{ return next; }

	/// Navigate to a sibling node with the given 'value'.
	TiXmlNode* NextSibling( const std::string& ) const;

	/** Convenience function to get through elements. 
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	TiXmlElement* NextSiblingElement() const;

	/** Convenience function to get through elements. 
		Calls NextSibling and ToElement. Will skip all non-Element
		nodes. Returns 0 if there is not another element.
	*/
	TiXmlElement* NextSiblingElement( const std::string& ) const;

	/// Convenience function to get through elements.
	TiXmlElement* FirstChildElement()	const;
	
	/// Convenience function to get through elements.
	TiXmlElement* FirstChildElement( const std::string& value ) const;

	/// Query the type (as an enumerated value, above) of this node.
	virtual int Type()	{ return type; }

	/** Return a pointer to the Document this node lives in. 
		Returns null if not in a document.
	*/
	TiXmlDocument* GetDocument() const;

	TiXmlDocument* ToDocument()	const	{ return ( type == DOCUMENT ) ? (TiXmlDocument*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlElement*  ToElement() const	{ return ( type == ELEMENT  ) ? (TiXmlElement*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlComment*  ToComment() const	{ return ( type == COMMENT  ) ? (TiXmlComment*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlUnknown*  ToUnknown() const	{ return ( type == UNKNOWN  ) ? (TiXmlUnknown*)  this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlText*	   ToText()    const	{ return ( type == TEXT     ) ? (TiXmlText*)     this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.
	TiXmlDeclaration* ToDeclaration() const	{ return ( type == DECLARATION ) ? (TiXmlDeclaration*) this : 0; } ///< Cast to a more defined type. Will return null not of the requested type.

	virtual TiXmlNode* Clone() const = 0;

  protected:
	TiXmlNode( NodeType type );
	virtual const char* Parse( const char* ) = 0;

	// The node is passed in by ownership. This object will delete it.
	TiXmlNode* LinkEndChild( TiXmlNode* addThis );

	// Figure out what is at *p, and parse it. Return a node if
	// successful, and update p.
	TiXmlNode* IdentifyAndParse( const char** p );

	void CopyToClone( TiXmlNode* target ) const	{ target->value = value; }

	TiXmlNode*		parent;		
	NodeType		type;
	
	TiXmlNode*		firstChild;
	TiXmlNode*		lastChild;

	std::string		value;
	
	TiXmlNode*		prev;
	TiXmlNode*		next;
};


/** An attribute is a name-value pair. Elements have an arbitrary
	number of attributes, each with a unique name.

	@note The attributes are not TiXmlNodes, since they are not
		  part of the tinyXML document object model. There are other
		  suggested ways to look at this problem.

	@note Attributes have a parent
*/
class TiXmlAttribute : public TiXmlBase
{
	friend class TiXmlAttributeSet;

  public:
	/// Construct an empty attribute.
	TiXmlAttribute() : prev( 0 ), next( 0 )	{}

	/// Construct an attribute with a name and value.
	TiXmlAttribute( const std::string& _name, const std::string& _value )	: name( _name ), value( _value ), prev( 0 ), next( 0 ) {}

	const std::string& Name()  const { return name; }		///< Return the name of this attribute.
	const std::string& Value() const { return value; }		///< Return the value of this attribute.

	void SetName( const std::string& _name )	{ name = _name; }		///< Set the name of this attribute.
	void SetValue( const std::string& _value )	{ value = _value; }		///< Set the value.

	/// Get the next sibling attribute in the DOM. Returns null at end.
	TiXmlAttribute* Next();
	/// Get the previous sibling attribute in the DOM. Returns null at beginning.
	TiXmlAttribute* Previous();

	bool operator==( const TiXmlAttribute& rhs ) const { return rhs.name == name; }
	bool operator<( const TiXmlAttribute& rhs )	 const { return name < rhs.name; }
	bool operator>( const TiXmlAttribute& rhs )  const { return name > rhs.name; }

	/*	[internal use] 
		Attribtue parsing starts: first letter of the name
						 returns: the next char after the value end quote
	*/	
	const char* Parse( const char* );

	// [internal use] 
	virtual void Print( FILE* fp, int depth );

	// [internal use]
	// Set the document pointer so the attribute can report errors.
	void SetDocument( TiXmlDocument* doc )	{ document = doc; }

  private:
	TiXmlDocument*	document;	// A pointer back to a document, for error reporting.
	std::string		name;
	std::string		value;

	TiXmlAttribute*	prev;
	TiXmlAttribute*	next;
};


/*	A class used to manage a group of attributes.
	It is only used internally, both by the ELEMENT and the DECLARATION.
	
	The set can be changed transparent to the Element and Declaration
	classes that use it, but NOT transparent to the Attribute 
	which has to implement a next() and previous() method. Which makes
	it a bit problematic and prevents the use of STL.

	This version is implemented with circular lists because:
		- I like circular lists
		- it demonstrates some independence from the (typical) doubly linked list.
*/
class TiXmlAttributeSet
{
  public:
	TiXmlAttributeSet();
	~TiXmlAttributeSet();

	void Add( TiXmlAttribute* attribute );
	void Remove( TiXmlAttribute* attribute );

	TiXmlAttribute* First() const	{ return ( sentinel.next == &sentinel ) ? 0 : sentinel.next; }
	TiXmlAttribute* Last()  const	{ return ( sentinel.prev == &sentinel ) ? 0 : sentinel.prev; }
	
	TiXmlAttribute*	Find( const std::string& name ) const;

  private:
	TiXmlAttribute sentinel;
};


/** The element is a container class. It has a value, the element name,
	and can contain other elements, text, comments, and unknowns.
	Elements also contain an arbitrary number of attributes.
*/
class TiXmlElement : public TiXmlNode
{
  public:
	/// Construct an element.
	TiXmlElement( const std::string& value );

	virtual ~TiXmlElement();

	/** Given an attribute name, attribute returns the value
		for the attribute of that name, or null if none exists.
	*/
	const std::string* Attribute( const std::string& name ) const;

	/** Given an attribute name, attribute returns the value
		for the attribute of that name, or null if none exists.
	*/
	const std::string* Attribute( const std::string& name, int* i ) const;

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const std::string& name, 
					   const std::string& value );

	/** Sets an attribute of name to a given value. The attribute
		will be created if it does not exist, or changed if it does.
	*/
	void SetAttribute( const std::string& name, 
					   int value );

	/** Deletes an attribute with the given name.
	*/
	void RemoveAttribute( const std::string& name );

	TiXmlAttribute* FirstAttribute()	{ return attributeSet.First(); }		///< Access the first attribute in this element.
	TiXmlAttribute* LastAttribute()		{ return attributeSet.Last(); }		///< Access the last attribute in this element.

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use] 
	virtual void Print( FILE* fp, int depth );

  protected:
	/*	[internal use] 
		Attribtue parsing starts: next char past '<'
						 returns: next char past '>'
	*/	
	virtual const char* Parse( const char* );
	const char* ReadValue( const char* p );

  private:
	TiXmlAttributeSet attributeSet;
};


/**	An XML comment.
*/
class TiXmlComment : public TiXmlNode
{
  public:
	/// Constructs an empty comment.
	TiXmlComment() : TiXmlNode( TiXmlNode::COMMENT ) {}
	virtual ~TiXmlComment()	{}

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use] 
	virtual void Print( FILE* fp, int depth );

  protected:
	/*	[internal use] 
		Attribtue parsing starts: at the ! of the !--
						 returns: next char past '>'
	*/	
	virtual const char* Parse( const char* );
};


/** XML text. Contained in an element.
*/
class TiXmlText : public TiXmlNode
{
  public:
	TiXmlText()  : TiXmlNode( TiXmlNode::TEXT ) {}
	virtual ~TiXmlText() {}


	// [internal use] Creates a new Element and returns it.
	virtual TiXmlNode* Clone() const;
	// [internal use] 
	virtual void Print( FILE* fp, int depth );
	// [internal use] 	
	bool Blank();	// returns true if all white space and new lines

	/*	[internal use] 
		Attribtue parsing starts: First char of the text
						 returns: next char past '>'
	*/	
	virtual const char* Parse( const char* );
};


/** XML Cdata section. Contained in an element.
 * Always start with <![CDATA[  and end with ]]>
*/
class TiXmlCData : public TiXmlText
{
        friend class TiXmlElement;
public:
        /// Constructor.
        TiXmlCData ()
        {}
        virtual ~TiXmlCData() {}

protected :
        virtual const char* Parse( const char* p );
};


/** In correct XML the declaration is the first entry in the file.
	@verbatim
		<?xml version="1.0" standalone="yes"?>
	@endverbatim

	TinyXml will happily read or write files without a declaration,
	however. There are 3 possible attributes to the declaration: 
	version, encoding, and standalone.

	Note: In this version of the code, the attributes are
	handled as special cases, not generic attributes, simply
	because there can only be at most 3 and they are always the same.
*/
class TiXmlDeclaration : public TiXmlNode
{
  public:
	/// Construct an empty declaration.
	TiXmlDeclaration()   : TiXmlNode( TiXmlNode::DECLARATION ) {}

	/// Construct.
	TiXmlDeclaration( const std::string& version, 
					  const std::string& encoding,
					  const std::string& standalone );

	virtual ~TiXmlDeclaration()	{}

	/// Version. Will return empty if none was found.
	const std::string& Version()		{ return version; }
	/// Encoding. Will return empty if none was found.
	const std::string& Encoding()		{ return encoding; }
	/// Is this a standalone document? 
	const std::string& Standalone()		{ return standalone; }

	// [internal use] Creates a new Element and returs it.
	virtual TiXmlNode* Clone() const;
	// [internal use] 
	virtual void Print( FILE* fp, int depth );

  protected:
	//	[internal use] 
	//	Attribtue parsing starts: next char past '<'
	//					 returns: next char past '>'
	
	virtual const char* Parse( const char* );

  private:
	std::string version;
	std::string encoding;
	std::string standalone;
};


/** Any tag that tinyXml doesn't recognize is save as an 
	unknown. It is a tag of text, but should not be modified.
	It will be written back to the XML, unchanged, when the file 
	is saved.
*/
class TiXmlUnknown : public TiXmlNode
{
  public:
	TiXmlUnknown() : TiXmlNode( TiXmlNode::UNKNOWN ) {}
	virtual ~TiXmlUnknown() {}

	// [internal use] 	
	virtual TiXmlNode* Clone() const;
	// [internal use] 	
	virtual void Print( FILE* fp, int depth );

  protected:
	/*	[internal use] 
		Attribute parsing starts: First char of the text
						 returns: next char past '>'
	*/	
	virtual const char* Parse( const char* );
};


/** Always the top level node. A document binds together all the
	XML pieces. It can be saved, loaded, and printed to the screen.
	The 'value' of a document node is the xml file name.
*/
class TiXmlDocument : public TiXmlNode
{
  public:
	/// Create an empty document, that has no name.
	TiXmlDocument();
	/// Create a document with a name. The name of the document is also the filename of the xml.
	TiXmlDocument( const std::string& documentName );
	
	virtual ~TiXmlDocument() {}

	/** Load a file using the current document value. 
		Returns true if successful. Will delete any existing
		document data before loading.
	*/
	bool LoadFile();
	/// Save a file using the current document value. Returns true if successful.
	bool SaveFile();
	/// Load a file using the given filename. Returns true if successful.
	bool LoadFile( const std::string& filename );
	/// Save a file using the given filename. Returns true if successful.
	bool SaveFile( const std::string& filename );
	/// Load a file using the given file pointer. Returns true if successful.
	bool LoadFile(FILE* fp);

	/// Parse the given null terminated block of xml data.
	const char* Parse( const char* );
	
	/// If, during parsing, a error occurs, Error will be set to true.
	bool Error()						{ return error; }
	/// Contains a textual (english) description of the error if one occurs.
	const std::string& ErrorDesc()		{ return errorDesc; }

	/// Write the document to a file -- usually invoked by SaveFile.
	virtual void Print( FILE* fp, int depth = 0 );
	/// Dump the document to standard out.
	void Print()										{ Print( stdout, 0 ); }
  
	// [internal use] 	
	virtual TiXmlNode* Clone() const;
	// [internal use] 	
	void SetError( int err ) {		assert( err > 0 && err < TIXML_ERROR_STRING_COUNT );
									error   = true; 
									errorId = err;
									errorDesc = errorString[ errorId ]; }

  private:
	bool error;
	int  errorId;	
	std::string errorDesc;
};


#endif

