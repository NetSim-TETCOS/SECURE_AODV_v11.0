#pragma once

typedef enum enu_node_type
{
	MetricsNode_Menu,
	MetricsNode_Table,
	MetricsNode_Text,
	MetricsNode_Link,
	MetricsNode_Plot,
	MetricsNode_Button,
}METRICS_NODE;

typedef enum enum_writer_position
{
	WriterPosition_Current,
	WriterPosition_Last,
	WriterPosition_Choose,
}WRITER_POSITION;

#ifndef _METRICS_NODE_CODE_
typedef void* PMETRICSNODE;
#else
typedef struct stru_metrics_node METRICSNODE, *PMETRICSNODE;
#endif

#ifndef _NETSIM_METRICS_WRITER_CODE_
typedef void* PMETRICSWRITER;
#else
typedef struct stru_metrics_writer METRICSWRITER, *PMETRICSWRITER;
#endif

/*Initialize a new metrics node.
  Input:
	1. Metrics node type
	2. Name of the node
	3. Second argument
		1. NULL in case of text and table
		2. onClick reference in case of link and button
		3. data_file_name in case of plot
*/
_declspec(dllexport) PMETRICSNODE init_metrics_node(METRICS_NODE type, char* name, char* secondarg);
//Delete/Free the metrics node.
_declspec(dllexport) void delete_metrics_node(PMETRICSNODE node);
//Add table heading to a metrics table.
_declspec(dllexport) void add_table_heading(PMETRICSNODE node, char* name, bool isShow, int subheadingcount, ...);
//Add table heading without subheading
_declspec(dllexport) void add_table_heading_special(PMETRICSNODE node, char* heading);
//Add formatted table row. Used for text row.
_declspec(dllexport) void add_table_row_formatted(bool issame, PMETRICSNODE node, char* format, ...);
//Used to add a node to table
_declspec(dllexport) void add_table_row(bool issame, PMETRICSNODE node, PMETRICSNODE row);

//Add a metrics node to menu item
_declspec(dllexport) void add_node_to_menu(PMETRICSNODE menu, PMETRICSNODE node);

//Write metrics node to xml file
_declspec(dllexport) void write_metrics_node(PMETRICSWRITER writer, WRITER_POSITION writerPosition, char* writer_name, PMETRICSNODE node);

/**Metrics writer**/
//Initialize the metrics writer.
_declspec(dllexport) PMETRICSWRITER init_xmlwriter(char* uri,char* name);
//Create new metrics writer position
_declspec(dllexport) void create_new_metrics_writer(PMETRICSWRITER writer, char* name);
//Add an element to xml file
_declspec(dllexport) void add_element(PMETRICSWRITER writer,
									  WRITER_POSITION writerPosition,
									  char* writer_name,
									  char* name,
									  void(*fncall)(PMETRICSWRITER writer, WRITER_POSITION writerPosition, char* writer_name, int argc, char* argv[]),
									  int argc,
									  char* argv[]);
//Close the xml writer
_declspec(dllexport) void close_xmlwriter(PMETRICSWRITER metricsWriter);
//Add an attribute to xml file
_declspec(dllexport) void add_attribute(PMETRICSWRITER writer, WRITER_POSITION writerPosition, char* writer_name, char* name, char* format, ...);


