/** @mainpage

<b> TinyXml </b>

TinyXml is a simple, small, C++ XML parser that can be easily 
integrating into other programs.


<b> What it does. </b>
	
In brief, TinyXml parses an XML document, and builds from that a 
Document Object Model that can be read, modified, and saved.

XML stands for "eXtensible Markup Language." It allows you to create 
your own document markups. Where HTML does a very good job of marking 
documents for browsers, XML allows you to define any kind of document 
markup, for example a document that describes a "to do" list for an 
organizer application. XML is a very structured and convenient format.
All those random file formats created to store application data can 
all be replaced with XML. One parser for everything.

There are different ways to access and interact with XML data.
TinyXml uses a Document Object Model, meaning the XML data is parsed
into a tree objects that can be browsed and manipulated, and then 
written back to disk. You can also construct an XML document from 
scratch with C++ objects and write this to disk.

TinyXml is designed to be easy and fast. It is one header and three cpp 
files. Simply add these to your project and off you go. There is an 
example to get you started. It is released under the ZLib license, 
so you can use it in open source or commercial code.

It attempts to be a flexible parser, but with truly correct and 
compliant XML output (with the exception of the character set,
below.) TinyXml should compile on any reasonably C++ 
system. It does not rely on exceptions or RTTI, and only uses the STL
string class.


<b> What it doesn't do. </b>

It doesn’t parse or use DTDs (Document Type Definitions) or XSL’s 
(eXtensible Stylesheet Language.) It is limited to the ASCII 
character set. There are other parsers out there (check out 
www.sourceforge.org, search for XML) that are much more fully 
featured. But they are also much bigger, take longer to set up in 
your project, have a higher learning curve, and have a more 
restrictive license. If you are working with browsers or have more 
complete XML needs, TinyXml is not the parser for you. 


<b> Code Status.  </b>

Currently in use, TinyXml is looking pretty stable. If you find
bugs, send them in and we'll get them straightened out as soon as possible.

It currently does not recognize "entity references", meaning special 
characters. This is a missing feature that will hopefully be 
included soon. Namely:
@verbatim
	&amp;	&
	&lt;	<
	&gt;	>
	&quot;	"
	&apos;	‘
@endverbatim

<b> Using and Installing </b>

To Compile and Run xmltest:

A Linux Makefile and a Windows Visual C++ .dsp file is provided. 
Simply compile and run. It will write the file demotest.xml to your 
disk and generate output on the screen. It also tests walking the
DOM by printing out the number of nodes found using different 
techniques.

The Linux makefile is very generic and will
probably run on other systems, but is only tested on Linux. Make
sure to run 'make depend' before you make, so you don't pick
up incorrect dependencies.


To Use in an Application:

Add tinyxml.cpp, tinyxml.h, tinyxmlerror.cpp, and tinyxmlparser.cpp to your 
project or make file. That's it! It should compile on any reasonably
compliant C++ system. You do not need to enable exceptions or
RTTI for TinyXml.


<b> Where it may go.  </b>

At this point, I'm focusing on tightening up remaining issues.
Bug fixes (though comfortably rare) and minor interface 
corrections. 

On the "it would be nice if..." list is:
- More intelligent (and consistent) parsing would
  be worthwhile; the parser is somewhat "organic" in its current form.
- Entities.

I'm not currently working on either; but would ethusiastically welcome
a patch!

In the future, I think it would be great if XSL and DTDs were added 
in some scalable way. So TinyXml would become a stand-alone core 
component of say MedXml (adding DTDs) and LargeXml( adding XSL.) :-) 


<b> How TinyXml works.  </b>

An example is probably the best way to go. Take:
@verbatim
	<?xml version="1.0" standalone=‘no’>
	<?-- Our to do list data -->
	<ToDo>
		<Item priority="1"> Go to the <bold>Toy store!</bold></Item>
		<Item priority="2"> Do bills</Item>
	</ToDo>
@endverbatim

It’s not much of a To Do list, but it will do. To read this file 
(say "demo.xml") you would create a document, and parse it in:
@verbatim
	TiXmlDocument doc( "demo.xml" );
	doc.LoadFile();
@endverbatim

And it’s ready to go. Now let’s look at some lines and how they 
relate to the DOM.

<?xml version="1.0" standalone=‘no’>

	The first line is a declaration, and gets turned into the
	TiXmlDeclaration class. It will be the first child of the
	document node.
	
	This is the only directive/special tag parsed by by TinyXml.
	Generally directive targs are stored in TiXmlUnknown so the 
	commands won’t be lost when it is saved back to disk.

<?-- Our to do list data -->

	A comment. Will become a TiXmlComment object.

<ToDo>

	The ToDo tag defines a TiXmlElement object. This one does not have 
	any attributes, but will contain 2 other elements, both of which 
	are items.

<Item priority="1"> 

	Creates another TiXmlElement which is a child of the "ToDo" element. 
	This element has 1 attribute, with the name ‘priority’ and the value 
	‘1’.

Go to the 

	A TiXmlText. This is a leaf node and cannot contain other nodes. 
	It is a child of the ‘Item" Element.

<bold>
	
	Another TiXmlElement, this one a child of the "Item" element.

Etc.

Looking at the entire object tree, you end up with:
@verbatim
TiXmlDocument				"demo.xml"
	TiXmlDeclaration		"version='1.0'" "standalone=‘no’"
	TiXmlComment			" Our to do list data"
	TiXmlElement			"ToDo"
		TiXmlElement		"Item"		Attribtutes: priority = 1
			TiXmlText		"Go to the "
			TiXmlElement    "bold"
				TiXmlText	"Toy store!"
	TiXmlElement			"Item"		Attributes: priority=2
		TiXmlText			"bills"
@endverbatim

<b> Contributors </b>

Thanks very much to everyone who sends suggestions, bugs, ideas, and 
encouragement. It all helps.

Major contributors to the project:
<ul>
<li> Lee Thomason wrote the original code and maintains the project.</li>
<li> Ayende Rahien presented code, ideas, and changes that became
the 1.1.0 version of TinyXml.</li>
<li> Ville Nurmi provided ideas, bugs, and feedback.</li>
</ul>

<b> Documentation </b>

The documentation is build with Doxygen, using the 'dox' 
configuration file.

<b>	License	</b>

TinyXml is released under the zlib license:

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

<b> References  </b>

The World Wide Web Consortium is the definitive standard body for 
XML, and there web pages contain huge amounts of information. I also 
recommend "XML Pocket Reference" by Robert Eckstein and published by 
O’Reilly.

<b> Contact Me: </b>

I’d appreciates your suggestions, and would love to know if you 
use TinyXml. I hope you enjoy it and find it useful.

Lee Thomason

leethomason@mindspring.com
*/
