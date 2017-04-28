#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"



int find(int parent[], int i)
{
	//printf("inside find function for %d \n", i);
	return parent[i];
}
 
int Union(int parent[], int v1, int v2)
{ 

// printf("inside union function: v1= %d \t v2= %d \n",v1,v2);
 if(v1 != v2)
 {
	//	printf("v1 !=v2\n ");
		parent[v2]=v1;
	//	printf("returning \n");
		return 1;
	}
 
 return 0;

}

// TODO: use schema values for edges as weights.
int sort_helper(const void* a, const void* b)
{
		struct edge* e1 = (struct edge*)a;
		struct edge* e2 = (struct edge*)b;
		return e1->tuple->s > e2->tuple->s;
}

graph_t sort_given_graph(graph_t g)
{

 assert(g!=NULL);
 edge_t e;
 edge_t f;
 edge_t temp;
 tuple_t t;
 int id1 =0; 
 int id2 =0;
 int e_weight = 0;
 int f_weight = 0;

 for (e = g->e; e!= NULL; e = e->next) 
 {

	if (e->tuple != NULL && 1)
	{
	 
	 e_weight = get_tuple_weight(e->tuple, g->el);
	// printf("e_weight is: %d \n", e_weight);
	
	}

	for(f = e->next; f!=NULL; f=f->next)
	{

		 if (f->tuple != NULL && 1)
		 {
			
			f_weight = get_tuple_weight(f->tuple, g->el);
			//printf("f_weight is: %d, e_weight is: %d \n", f_weight, e_weight);
		 
		 }

		if(e_weight > f_weight)
		 {
					temp = e;
					//edge_print(temp);
					id1 = e->id1;
					id2 = e->id2;
					t = e-> tuple;

					e->id1 = f->id1;
					e->id2 = f->id2;
					e->tuple = f->tuple;
					//update edge e's weight since it changed.
					e_weight = get_tuple_weight(e->tuple, g->el);

					f->id1 = id1;
					f->id2 = id2;
					f->tuple = t;
	 		}

		//	printf(" e's ids: %d %d\n",e->id1, e->id2 );
		//	printf(" f's ids: %d %d\n",f->id1, f->id2 );
		}	
	
 }

 return g;
 //graph_print(g,1);
 
}

// A utitility function very similar to cli_graph_new function. 
graph_t init_graph(int id1)
{

 graph_t temp;
 vertex_t v;
	
 // Create first vertex in graph /
 v = (vertex_t) malloc(sizeof(struct vertex));
 assert (v != NULL);
 vertex_init(v);
 v->id = id1;

 //printf("init vertex\n");
 // Create new graph /
 temp = (graph_t) malloc(sizeof(struct graph));
 assert (temp != NULL);
 graph_init(temp);
 graph_insert_vertex(temp, v);

// printf("init the graph:\n");
 if (graphs == NULL)
	current = temp;

 cli_graphs_insert(temp);

 return temp;
}

void run_kruskal(graph_t g, int num_vertices, int num_edges)
{
 int parent[20];
 int V = num_vertices;
 int e=0;
 int total_cost =0;

 //storing the final MST edges.
 edge_t current_edge, ed;
 graph_t result_graph =NULL;
 graph_t sorted_graph;
 vertex_t v;
 vertex_t w;
 
 sorted_graph = sort_given_graph(g);

 //graph_print(sorted_graph,1);
 //printf("sorted \n");

 
 //initializing parent vector
 for (int j = 1; j <= V; ++j)
 {
			parent[j] = j;
	}

//printf("initialized parent array \n");
		
	current_edge = sorted_graph->e;
		// terminating condition for mst
	while(e < (V-1))
	{
		 
		 	//printf("found %d edges in the MST: %d",e);
		 
		 	//printf(" looking to add edge\n");

			int x = find(parent, current_edge->id1);
			int y = find(parent, current_edge->id2);

			//printf("x= %d \t y = %d",x,y);

				// If including this edge does't cause cycle, include it
				// in result and increment the index of result for next edge
			
			if (Union(parent, x, y))
			{
				//printf("adding edge: ");
				//add edge to final result
				//edge_print(current_edge);

				total_cost += get_tuple_weight(current_edge->tuple, sorted_graph->el);	
				
				if(result_graph == NULL)
				{
					//printf("result graph is null, init it.\n");
					result_graph = init_graph(current_edge->id1);

					// add vertex id2 now. 

					v = (vertex_t) malloc(sizeof(struct vertex));
					assert (v != NULL);
					vertex_init(v);
					v->id = current_edge->id2;

					graph_insert_vertex(result_graph, v);

					result_graph->se = sorted_graph->se;

					ed = (edge_t) malloc(sizeof(struct edge));
					assert (ed != NULL);
					edge_init(ed);
					edge_set_vertices(ed, current_edge->id1, current_edge->id2);

					ed->tuple = current_edge->tuple;
					graph_insert_edge(result_graph, ed);
				}
			
				else
				{
					v = graph_find_vertex_by_id(result_graph, current_edge->id1);

					w = graph_find_vertex_by_id(result_graph, current_edge->id2);

				 // either of v or w would already be existing in the graph -- due to the rules of grdb which
				 // does not allow two new vertices to be added to the graph.
				 if (v == NULL) 
				 {
						v = (vertex_t) malloc(sizeof(struct vertex));
						assert (v != NULL);
						vertex_init(v);
						v->id = current_edge->id1;

						graph_insert_vertex(result_graph, v);

					} 
					else if (w == NULL) 
					{
							w = (vertex_t) malloc(sizeof(struct vertex));
							assert (w != NULL);
							vertex_init(w);
							w->id =  current_edge->id2;

							graph_insert_vertex(result_graph, w);
					}


					ed = (edge_t) malloc(sizeof(struct edge));
					assert (ed != NULL);
					edge_init(ed);
					edge_set_vertices(ed, current_edge->id1, current_edge->id2);
					ed->tuple = current_edge->tuple;
					graph_insert_edge(result_graph, ed);
						 
				}
				
				

				
						// increase edge count in the spanning tree
				e++;
					 
			}
				// Else discard the next_edge
			current_edge = current_edge->next;

		} 
		
		printf("result graph: \n");
		graph_print(result_graph,1);

		printf("\n total cost: %d \n", total_cost);
		
		return;
}

void cli_kruskal(char cmdline, int pos)
{
 printf("Kruskals algorithm: \n");

		// verify that there is a graph before running kruskal
 assert (graphs != NULL);

 graph_t g;
 vertex_t v;
 edge_t e;
 // graph count
 int cnt = 0;
 int vertex_count = 0;
 int edge_count = 0;

 for (g = graphs; g != NULL; g = g->next, cnt++) 
 {
		if (g == current){
	 	//printf("current graph: \n");
	 	for(v = g->v; v != NULL; v = v->next) 
	 		vertex_count++; 
	 	//printf("vertex count is: %d \n", vertex_count);
	 	for (e= g->e; e!= NULL; e=e->next)
	 		edge_count++;
	 	//printf("edge count is: %d \n", edge_count);
	 	run_kruskal(g, vertex_count, edge_count);

 }
}
}