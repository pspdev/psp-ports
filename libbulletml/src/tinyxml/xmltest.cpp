#include <stdlib.h>
#include "tinyxml.h"

//
// This file demonstrates some basic functionality of TinyXml.
// Note that the example is very contrived. It presumes you know
// what is in the XML file. But it does test the basic operations,
// and show how to add and remove nodes.
//

int main()
{
	//
	// We start with the 'demoStart' todo list. Process it. And
	// should hopefully end up with the todo list as illustrated.
	//
	const char* demoStart = 
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<!-- Our to do list \n data -->"
		"<ToDo>\n"
		"<Item priority=\"1\" distance='close'> Go to the <bold>Toy store!</bold></Item>"
		"<Item priority=\"2\" distance='none'><![CDATA[ Do bills   ]]></Item>"
		"<Item priority=\"2\" distance='far'> Look for Evil Dinosaurs! </Item>"
		"</ToDo>";

	/* What the todo list should look like after processing.

		<?xml version="1.0" standalone="no" ?>
		<!-- Our to do list data -->
		<ToDo>
		    <Item priority="2" distance="close"> Go to the
		        <bold>Toy store!
		        </bold>
		    </Item>
		    <Item priority="1" distance="far"> Talk to:
		        <Meeting where="School">
		            <Attendee name="Marple" position="teacher" />
		            <Attendee name="Voo" position="counselor" />
		        </Meeting>
		        <Meeting where="Lunch" />
		    </Item>
		    <Item priority="2" distance="here"> Do bills
		    </Item>
		</ToDo>
	*/

	// The example parses from the character string (above):

	{
		// Write to a file and read it back, to check file I/O.

		TiXmlDocument doc( "demotest.xml" );
		doc.Parse( demoStart );

		if ( doc.Error() )
		{
			printf( "Error in %s: %s\n", doc.Value().c_str(), doc.ErrorDesc().c_str() );
			exit( 1 );
		}
		doc.SaveFile();
	}

	TiXmlDocument doc( "demotest.xml" );
	doc.LoadFile();

	printf( "** Demo doc read from disk: ** \n\n" );
	doc.Print( stdout );

	TiXmlNode* node = 0;
	TiXmlElement* todoElement = 0;
	TiXmlElement* itemElement = 0;

	// --------------------------------------------------------
	// An example of changing existing attributes, and removing
	// an element from the document.
	// --------------------------------------------------------

	// Get the "ToDo" element.
	// It is a child of the document, and can be selected by name.
	node = doc.FirstChild( "ToDo" );
	assert( node );
	todoElement = node->ToElement();
	assert( todoElement  );

	// Going to the toy store is now our second priority...
	// So set the "priority" attribute of the first item in the list.
	node = todoElement->FirstChild();
	assert( node );
	itemElement = node->ToElement();
	assert( itemElement  );
	itemElement->SetAttribute( "priority", 2 );

	// Change the distance to "doing bills" from
	// "none" to "here". It's the next sibling element.
	itemElement = itemElement->NextSiblingElement();
	itemElement->SetAttribute( "distance", "here" );

	// Remove the "Look for Evil Dinosours!" item.
	// It is 1 more sibling away. We ask the parent to remove
	// a particular child.
	itemElement = itemElement->NextSiblingElement();
	todoElement->RemoveChild( itemElement );

	itemElement = 0;

	// --------------------------------------------------------
	// What follows is an example of created elements and text
	// nodes and adding them to the document.
	// --------------------------------------------------------

	// Add some meetings.
	TiXmlElement item( "Item" );
	item.SetAttribute( "priority", "1" );
	item.SetAttribute( "distance", "far" );

	TiXmlText text;
	text.SetValue( "Talk to:" );

	TiXmlElement meeting1( "Meeting" );
	meeting1.SetAttribute( "where", "School" );

	TiXmlElement meeting2( "Meeting" );
	meeting2.SetAttribute( "where", "Lunch" );

	TiXmlElement attendee1( "Attendee" );
	attendee1.SetAttribute( "name", "Marple" );
	attendee1.SetAttribute( "position", "teacher" );

	TiXmlElement attendee2( "Attendee" );
	attendee2.SetAttribute( "name", "Voo" );
	attendee2.SetAttribute( "position", "counselor" );

	// Assemble the nodes we've created:
	meeting1.InsertEndChild( attendee1 );
	meeting1.InsertEndChild( attendee2 );

	item.InsertEndChild( text );
	item.InsertEndChild( meeting1 );
	item.InsertEndChild( meeting2 );

	// And add the node to the existing list after the first child.
	node = todoElement->FirstChild( "Item" );
	assert( node );
	itemElement = node->ToElement();
	assert( itemElement );

	todoElement->InsertAfterChild( itemElement, item );

	printf( "\n** Demo doc processed: ** \n\n" );
	doc.Print( stdout );

	// --------------------------------------------------------
	// Different ways to walk the XML document.
	// --------------------------------------------------------

	int count = 0;
	TiXmlElement*	element;

	// Walk all the top level nodes of the document.
	count = 0;
	for( node = doc.FirstChild();
		 node;
		 node = node->NextSibling() )
	{
		count++;
	}
	printf( "The document contains %d top level nodes. (3)\n", count );


	// Walk all the top level nodes of the document,
	// using a different sytax.
	count = 0;
	for( node = doc.IterateChildren( 0 );
		 node;
		 node = doc.IterateChildren( node ) )
	{
		count++;
	}
	printf( "The document contains %d top level nodes. (3)\n", count );


	// Walk all the elements in a node.
	count = 0;
	for( element = todoElement->FirstChildElement();
		 element;
		 element = element->NextSiblingElement() )
	{
		count++;
	}
	printf( "The 'ToDo' element contains %d elements. (3)\n", count );


	// Walk all the elements in a node by value.
	count = 0;
	for( node = todoElement->FirstChild( "Item" );
		 node;
		 node = node->NextSibling( "Item" ) )
	{
		count++;
	}
	printf( "The 'ToDo' element contains %d nodes with the value of 'Item'. (3)\n", count );
	
	/*
	for( int i=0; i<1000; i++ )	
		doc.LoadFile( "SmallRuleset1.xml" );
	doc.SaveFile( "smalltest.xml" );
 	*/
	return 0;
}

