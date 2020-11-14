#include "gtktemplate.h"
#include <dirent.h>
#include <math.h>
#include <cairo.h>
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <time.h> 
// compile with gcc -Wall -g -lm carnieinterface.c -o carnieinterface `pkg-config --cflags --libs gtk+-3.0`
//interactive debug GTK_DEBUG=interactive ./carnieinterface
// debug with G_MESSAGES_DEBUG=all ./carnieinterface

gfloat f (gfloat x);
static gboolean fill_progress ();
void delay(int number_of_seconds);
void read_from_output(GtkWidget *progress_box);
void packboxes(int i);
void Exit();
void createfilechoosers(GtkWidget *widget, gpointer data);
void Age();
void Gender();
void prefilter();
void process();
void train();
void guess();
void eval();
void export();
void bn();
void inception();
void singlecrop();
void multicrop();
void adadelta();
void momentum();
void default_model();
void create_progress_window(int i);
void create_graph(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void create_graph_window();
void preview_thumbnail(GtkWidget *widget, cairo_t *cr, gpointer user_data);
void eval_valid();
void eval_train();
void about();
void addlog();
void runonce();
void cascade();
void yolo();
void dlib();
void helpbox();
void prune();
void setdefaults();
void runcommand(char command[]);
void createnotebookpage(int i, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, gchar *chooserlabels[], void *choosercallbacks, 
																									size_t chooserarr_size, gchar *labeltext[]);
struct widgets
{
	GtkWidget *window;
	GtkWidget *buttonbox;
	GtkWidget *radiobox;
	GtkWidget *filechoosers;
	GtkWidget *rootbutton;
	GtkWidget *connectwindow;
	GtkWidget *connectinglabel;
	GtkWidget *chooservbox;
	GtkWidget *notebook;
	GtkWidget *frame;
	GtkWidget *table;
	GtkWidget *framebox;
	GtkWidget *entrygrid;
	GtkWidget *entries[9];
	GtkWidget *entrylabelgrid;
	GtkWidget *model_box;
	GtkWidget *progress_bar;
	GtkWidget *graph;
	GtkWidget *graphbox;
	GtkWidget *thumbnail;
	GtkWidget *eval_entries[8];
	GtkWidget *eval_box;
	GtkWidget *eval_entry_grid;
	GtkWidget *eval_entry_label_grid;
	GtkWidget *eval_valid_box;
	GtkWidget *labelgrid;
	GtkWidget *choosers;
	GtkWidget *guess_entry_grid;
	GtkWidget *guess_entries[4];
	GtkWidget *guess_label_grid;
	GtkWidget *crop_box;
	GtkWidget *process_entry_grid;
	GtkWidget *process_entry_label_grid;
	GtkWidget *process_entries[3];
	GtkWidget *helpchooser;
	GtkWidget *train_checkbox;
	GtkWidget *runonce_checkbox;
	GtkWidget *face_detect_box;
	GtkWidget *optim_box;
	GtkWidget *export_entry_grid;
	GtkWidget *export_entry_label_grid;
	GtkWidget *export_entries[2];
	GtkWidget *help_view;
    GtkWidget *scrolledwindow;
    GtkWidget *text_entry;
    GtkWidget *help_grid;
}gwidget;

struct variables
{
	char *inputfile1;
	char *inputfile2;
	char *filenames[100];
	int placement;
	char *checkpoint;
	char *age_gender;
	int batch_size;
	char model;
	char *model_type;
	char *log;
	char *eval_data;
	char *optim;
	char *face_detect_type;
	char *crop;
	char *runonce;
	const gchar *process_entries[3];
	const gchar *train_entries[9];
	const gchar *guess_entries[4];
	const gchar *eval_entries[8];
	const gchar *export_entries[2];
}location;

gchar *prefilter_labels[] = {"Images Folder\n", "Model\n", "Target\n"};
gchar *prefilter_butt_labels[]= {"Pre-Filter", "Exit"};
gchar *prefilter_choose_labels[] = {"Open", "Open", "Open"}; 
gchar *process_labels[] = {"Images Folder\n", "Fold\n","Val\n", "Train\n", "Output\n"};
gchar *process_butt_labels[] = {"Process", "Exit"};
gchar *process_choose_labels [] = {"Open", "Open", "Open", "Open", "Open"};
gchar *process_entry_labels[] = {"Train Shards\n", "Valid Shards\n", "Threads\n"};
gchar *train_labels[] = {"Train Folder\n", "Pre-Checkpoint\n", "Checkpoint\n", "Pre-Model"};
gchar *train_butt_labels[] = {"Train", "Exit"};
gchar *train_choose_labels [] = {"Open"};
gchar *guess_labels[] = {"Model\n", "Images Folder\n","Checkpoint\n", "Face Detection Model\n"};
gchar *guess_butt_labels[] = {"Guess", "Exit"};
gchar *guess_choose_labels [] = {"Open", "Open", "Open", "Open"};
gchar *guess_entry_labels[] = {"Requested Step\n", "Device ID\n", "Face Detection Type\n", "Target\n"};
gchar *eval_labels[] = {"Run Folder\n", "Eval Folder\n", "Checkpoint Folder\n"};
gchar *eval_choose_labels[] = {"Open", "Open", "Open"};
gchar *eval_butt_labels[] = {"Eval", "Exit"};
gchar *eval_valid_labels[] = {"Valid", "Train"};
gchar *eval_entry_labels[] = {"Run ID\n", "Device ID\n", "Batch size\n", "Image Size\n", "Requested Step\n", "Interval\n", "Examples\n", "Threads\n"};
gchar *export_labels[] = {"Checkpoint\n", "Output\n", "Model\n"};
gchar *export_chooser_labels[] = {"Open", "Open", "Open"};
gchar *export_butt_labels[] = {"Export", "Exit"};
gchar *export_entry_labels[] = {"Model Version\n", "Requested Step\n"};
gchar *help_butt_labels[] = {"Help\n", "About\n"};
gchar *help_labels[] = {"Help\n", "About\n"};
gchar *help_chooser_label[] = {"help"};
gchar *face_detect_type[] = {"Cascade", "YOLO", "dlib"};
gchar *radiolabels[] = {"Gender", "Age"};
gchar *optim_labels[] = {"Momentum", "Adadelta"};
gchar *crop_labels [] = {"Single Crop", "Multi-Crop"};
gchar *entry_labels[] = {"Batch Size\n","Max Steps\n", "Image Size\n", "Learning Rate\n", "Learning Decay\n", "Steps Per Decay\n", "Drop Out\n", "Epochs\n", "Threads\n"};
gchar *model_labels[] = {"default", "bn", "inception", "prune"};
void *prefilter_chooser[] = {createfilechoosers, createfilechoosers, createfilechoosers};
void *prefilter_butt_callbacks[] = {prefilter, Exit};
void *process_chooser[] = {createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers};
void *process_butt_callbacks[] = {process, Exit};
void *train_chooser[] = {createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers};
void *train_butt_callbacks[] = {train, Exit};
void *guess_chooser[] = {createfilechoosers, createfilechoosers, createfilechoosers, createfilechoosers};
void *guess_face_callbacks [] = {cascade, yolo, dlib};
void *guess_butt_callbacks[] = {guess, Exit};
void *eval_chooser[] = {createfilechoosers, createfilechoosers, createfilechoosers};
void *eval_butt_callbacks[] = {eval, exit};
void *eval_valid_callbacks[] = {eval_valid, eval_train}; 
void *export_butt_callbacks[] = {export, exit};
void *export_chooser_callbacks[] = {createfilechoosers, createfilechoosers};
void *help_butt_callbacks[] = {createfilechoosers};
void *help_callbacks[] = {createfilechoosers};
void *radiocallback[] = {Gender, Age};
void *model_radio[] = {default_model, bn, inception, prune};
void *crop_callbacks[] = {singlecrop, multicrop};
void *optim_radio[] = {momentum, adadelta};
size_t rlabels_size = arraysize(radiolabels);
size_t prefilter_size = arraysize(prefilter_labels);
size_t prefilter_butt_size = arraysize(prefilter_butt_labels);
size_t process_butt_size = arraysize(process_butt_labels);
size_t process_entry_size = arraysize(process_entry_labels);
size_t train_butt_size = arraysize(train_butt_labels);
size_t optim_size = arraysize(optim_labels);
size_t guess_butt_size = arraysize(guess_butt_labels);
size_t guess_entry_size = arraysize(guess_entry_labels);
size_t face_detect_size = arraysize(face_detect_type);
size_t crop_len = arraysize(crop_labels);
size_t eval_butt_size = arraysize(eval_butt_labels);
size_t eval_valid_size = arraysize(eval_valid_labels);
size_t entry_len = arraysize(entry_labels);
size_t process_size = arraysize(process_labels);
size_t train_size = arraysize(train_labels);
size_t guess_size = arraysize(guess_labels);
size_t model_size = arraysize(model_labels);
size_t eval_size = arraysize(eval_labels);
size_t help_size = arraysize(help_chooser_label);
size_t help_butt_size = arraysize(help_butt_labels);
size_t eval_entry_size = arraysize(eval_entry_labels);
size_t export_size = arraysize(export_chooser_labels);
size_t export_butt_size = arraysize(export_butt_labels);
size_t export_entry_size = arraysize(export_entry_labels);
gchar *pageheads[] = {"Prefilter","Preprocess", "Train", "Guess", "Evaluate","Export", "Help"};
const gchar *style[] = {"window_style", "label_style", "button_style", "help_style"};
gdouble fraction;

int main(int argc, char *argv [])
{
	gtk_init(&argc, &argv); //starting gtk 

	gwidget.window = createwindow("Carnie Interface", GTK_WIN_POS_CENTER);
	gwidget.notebook = createnotebook(gwidget.window);
	createnotebookpage(0, prefilter_butt_labels, prefilter_butt_callbacks, prefilter_butt_size, prefilter_choose_labels, prefilter_chooser, prefilter_size, prefilter_labels);
	createnotebookpage(1, process_butt_labels, process_butt_callbacks, process_butt_size, process_choose_labels, process_chooser, process_size, process_labels);
	createnotebookpage(2, train_butt_labels, train_butt_callbacks, train_butt_size, process_choose_labels, train_chooser, train_size, train_labels);
	createnotebookpage(3, guess_butt_labels, guess_butt_callbacks, guess_butt_size, process_choose_labels, guess_chooser, guess_size, guess_labels);
	createnotebookpage(4, eval_butt_labels, eval_butt_callbacks, eval_butt_size, eval_choose_labels, eval_chooser, eval_size, eval_labels);
	createnotebookpage(5, export_butt_labels, export_butt_callbacks, export_butt_size, export_chooser_labels, export_chooser_callbacks, export_size, export_labels);
	createnotebookpage(6, NULL, NULL, 0, NULL, NULL, 0, NULL);

	show_and_destroy(gwidget.window);
}

void createnotebookpage(int i, gchar *buttonlabels[], void *buttoncallbacks[], size_t butt_size, gchar *chooserlabels[], void *choosercallbacks, 
													size_t chooserarr_size, gchar *labeltext[])
{
	gwidget.frame = create_frame_with_pagehead(gwidget.notebook, pageheads, i);
        gwidget.labelgrid = createlabels(labeltext, chooserarr_size);
        gwidget.buttonbox = createsinglesizegrid(buttonlabels, buttoncallbacks, 1, butt_size);
	gwidget.choosers = createsinglesizegrid(chooserlabels, choosercallbacks, chooserarr_size,1);

	switch(i)
	{
		case 0:
		gwidget.face_detect_box = createradiobuttons(face_detect_type, guess_face_callbacks, face_detect_size);
		packboxes(0);
		break;

		case 1:
		gwidget.process_entry_grid = gtk_grid_new();
		gwidget.process_entry_label_grid = createlabels(process_entry_labels, process_entry_size);
		create_entries(process_entry_size, gwidget.process_entries, gwidget.process_entry_grid);
		packboxes(i);
		break;
		
		case 2: 
		gwidget.entrygrid = gtk_grid_new();
		gwidget.entrylabelgrid = createlabels(entry_labels, entry_len);
		gwidget.graph = gtk_drawing_area_new();
		create_entries(entry_len, gwidget.entries, gwidget.entrygrid);
		gtk_widget_set_size_request(gwidget.graph, 600, 600); //sets the size of the buttons
		g_signal_connect (G_OBJECT (gwidget.graph),"draw", G_CALLBACK (create_graph), NULL);
		gwidget.optim_box = createradiobuttons(optim_labels, optim_radio, optim_size);
		gwidget.model_box = createradiobuttons(model_labels, model_radio, model_size);
		gwidget.train_checkbox = create_checkbox(gwidget.window,"Log", FALSE, addlog);
	    packboxes(i);
	    break;
	    
	    case 3:
	    gwidget.thumbnail = gtk_drawing_area_new();
		gtk_widget_set_size_request(gwidget.thumbnail, 600, 600); //sets the size of the buttons
		g_signal_connect (G_OBJECT (gwidget.thumbnail), "draw", G_CALLBACK (preview_thumbnail), NULL);
		gwidget.guess_entry_grid = gtk_grid_new();
		gwidget.guess_label_grid = createlabels(guess_entry_labels, guess_entry_size);
    	create_entries(guess_entry_size, gwidget.guess_entries, gwidget.guess_entry_grid);
    	gwidget.radiobox = createradiobuttons(radiolabels,radiocallback, rlabels_size);
    	gwidget.model_box = createradiobuttons(model_labels, model_radio, model_size);
    	gwidget.face_detect_box = createradiobuttons(face_detect_type, guess_face_callbacks, face_detect_size);
    	gwidget.crop_box= createradiobuttons(crop_labels, crop_callbacks, crop_len);
		packboxes(i);
		break;
		
		case 4:
		gwidget.eval_entry_grid = gtk_grid_new();
		gwidget.eval_entry_label_grid = createlabels(eval_entry_labels, eval_entry_size);
		create_entries(eval_entry_size, gwidget.eval_entries, gwidget.eval_entry_grid);
		gwidget.model_box = createradiobuttons(model_labels, model_radio, model_size);
		gwidget.eval_valid_box = createradiobuttons(eval_valid_labels, eval_valid_callbacks,  eval_valid_size);
		gwidget.runonce_checkbox = create_checkbox(gwidget.window, "Run Once", FALSE, runonce);
		packboxes(i);
		break;
		
		case 5:
		gwidget.export_entry_grid = gtk_grid_new();
		gwidget.export_entry_label_grid = createlabels(export_entry_labels, export_entry_size);
		create_entries(export_entry_size, gwidget.export_entries, gwidget.export_entry_grid);
		gwidget.radiobox = createradiobuttons(radiolabels,radiocallback, rlabels_size);
		gwidget.model_box = createradiobuttons(model_labels, model_radio, model_size);
		packboxes(i);
		break;

		case 6:
		helpbox();
		packboxes(i);
		break;
		
		default: 
		packboxes(i);
		break;
	}
}

void packboxes(int i)
{
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	GtkWidget *chooserbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 
	GtkWidget *guessVbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	GtkWidget *graphbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1); //creates a vbox without autosizing 
	GtkWidget *entrybox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1); //creates a vbox without autosizing 

	switch(i)
	{
		case 0:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
		gtk_box_pack_start(GTK_BOX(vbox),  hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.face_detect_box, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 1: 
		gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
		gtk_box_pack_start(GTK_BOX(vbox),  hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  entrybox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.process_entry_label_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.process_entry_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 2: 
		gtk_container_add (GTK_CONTAINER (gwidget.frame), hbox);
		gtk_box_pack_start(GTK_BOX(hbox),  vbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  entrybox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.entrylabelgrid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.entrygrid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  chooserbox, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.train_checkbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.model_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.optim_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(hbox), graphbox, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(graphbox), gwidget.graph, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 3:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
		gtk_box_pack_start(GTK_BOX(vbox),  hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(hbox),  guessVbox, FALSE, FALSE, 0); 
		gtk_box_pack_start(GTK_BOX(guessVbox),  entrybox, FALSE, FALSE, 0); 
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.guess_label_grid, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.guess_entry_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(hbox),  gwidget.thumbnail, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  gwidget.radiobox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  gwidget.model_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  gwidget.face_detect_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  gwidget.crop_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(guessVbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 4:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
		gtk_box_pack_start(GTK_BOX(hbox),  entrybox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.eval_entry_label_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.eval_entry_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.eval_valid_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.model_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.runonce_checkbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 5:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), vbox);
		gtk_box_pack_start(GTK_BOX(vbox),  hbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  entrybox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.export_entry_label_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(entrybox),  gwidget.export_entry_grid, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  chooserbox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.labelgrid, FALSE, FALSE, 3); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(chooserbox),  gwidget.choosers, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.radiobox, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.model_box, FALSE, FALSE, 0); //packs the display into the vbox
		gtk_box_pack_start(GTK_BOX(vbox),  gwidget.buttonbox, FALSE, FALSE, 0); //packs the display into the vbox
		break;

		case 6:
		gtk_container_add (GTK_CONTAINER (gwidget.frame), gwidget.scrolledwindow);
		break;
		default:
		break;
	}
}

void createfilechoosers(GtkWidget *widget, gpointer data) 
{
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	gint res;
	GtkWindow *new_window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gwidget.filechoosers = gtk_file_chooser_dialog_new ("Open File", new_window, action, ("_Cancel"), GTK_RESPONSE_CANCEL, ("_Open"), GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_create_folders ((GtkFileChooser*)gwidget.filechoosers, TRUE);

	res = gtk_dialog_run (GTK_DIALOG (gwidget.filechoosers));
	if (res == GTK_RESPONSE_ACCEPT)
	  {
	   GtkFileChooser *chooser = GTK_FILE_CHOOSER(gwidget.filechoosers);
	   int i = (int) (long) data;
	   location.filenames[i] =  gtk_file_chooser_get_filename(chooser);
	  }
	  gtk_widget_destroy (gwidget.filechoosers);
	  //set_thumbnail(location.filenames[0], );
}

void read_from_output(GtkWidget *progress_box)
   {
	char temp[512];
	char str[512] = "";
	int number = 0;
	GtkWidget *grid = gtk_grid_new();
	GtkWidget *label[3];

	for(int j = 0; j<4; j++)
		{	
			label[j] = gtk_label_new("0");
			gtk_grid_attach(GTK_GRID(grid), label[j], 0, j, 1, 1); //sets the defaults for creating each table button
		}
	set_spacing(grid, 4, 20);
	gtk_box_pack_start(GTK_BOX(progress_box),  grid, TRUE, FALSE, 1); //packs the display into the vbox

	while(number <= 40000)
	{
		FILE *fp = fopen ("output.txt", "r+");

		while(fgets(temp, 512, fp) != NULL)
	 {
		if((strstr(temp, str)) != NULL)
		 {
			number += 10;
			snprintf(str, sizeof(str), "step %d", number);
			gtk_label_set_text(GTK_LABEL(label[0]), str);
		}
		if((strstr(temp, str)) == NULL)
		{
			delay(5);
		}
	}
	fclose(fp);
	}
}

void create_progress_window(int i)
{
	GtkWindow *new_window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gchar *training_labels[] = {"Steps Completed\n", "Examples per second\n", "Loss\n", "Time Remaining\n"};
	size_t training_size = arraysize(training_labels);
	GtkWidget *progress_box;
	GtkWidget *progress_hbox;
	progress_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	progress_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

	switch(i)
	{
		case 0: gtk_window_set_title(GTK_WINDOW(new_window), "Prefiltering in progress"); //sets a window title 
		break;
		case 1: gtk_window_set_title(GTK_WINDOW(new_window), "Preprocessing in progress"); //sets a window title
		break;
		case 2: gtk_window_set_title(GTK_WINDOW(new_window), "Training in progress"); //sets a window title
		GtkWidget *grid = createlabels(training_labels, training_size);
		gtk_box_pack_start(GTK_BOX(progress_hbox),  grid, FALSE, FALSE, 1); //packs the display into the vbox
		GThread *searchthread;
		searchthread = g_thread_new(NULL, (GThreadFunc) read_from_output, (GtkWidget *) progress_hbox);
		break;
		case 3: gtk_window_set_title(GTK_WINDOW(new_window), "Guessing in progress"); //sets a window title
		break;
		case 4: gtk_window_set_title(GTK_WINDOW(new_window), "Evaluating in progress"); //sets a window title
		break;
		case 5: gtk_window_set_title(GTK_WINDOW(new_window), "Exporting in progress"); //sets a window title 
		break;  
	}

	gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 300);
	gtk_window_set_resizable (GTK_WINDOW(new_window), FALSE);
	gtk_window_set_position(GTK_WINDOW(new_window), GTK_WIN_POS_CENTER); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(new_window), 5); //sets the border size of the window
	
	gwidget.progress_bar = gtk_progress_bar_new ();
	g_timeout_add (500, fill_progress, GTK_PROGRESS_BAR (gwidget.progress_bar));

	gtk_container_add (GTK_CONTAINER (new_window), progress_box);
	gtk_box_pack_start(GTK_BOX(progress_box),  progress_hbox, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(progress_box),  gwidget.progress_bar, FALSE, FALSE, 0); //packs the display into the vbox

	add_context(style[1], gwidget.progress_bar);
	gtk_widget_show_all((GtkWidget *)new_window); //shows all widgets 
}

void create_graph_window() //this works for creating the base of the graph, still have to poll 
{						   //the output file for dataand plot points for the graph 
	GtkWindow *new_window = (GtkWindow *)gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 300);
	gtk_window_set_resizable (GTK_WINDOW(new_window), FALSE);
	gtk_window_set_position(GTK_WINDOW(new_window), GTK_WIN_POS_CENTER); //opens the window in the center of the screen
	gtk_container_set_border_width(GTK_CONTAINER(new_window), 5); //sets the border size of the window
	add_context(style[0], (GtkWidget *)new_window);

	GtkWidget *graph = gtk_drawing_area_new();
	gtk_widget_set_size_request(graph, 600, 600); //sets the size of the buttons
	g_signal_connect (G_OBJECT (graph),"draw", G_CALLBACK (create_graph), NULL);

	gtk_container_add (GTK_CONTAINER (new_window), graph);

	gtk_widget_show_all((GtkWidget *)new_window); //shows all widgets 
}

void create_graph(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    GdkRectangle da;            /* GtkDrawingArea size */
    gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
    gdouble clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;

    /* Determine GtkDrawingArea dimensions */
	da.width = 480;
	da.height = 480;

    /* Draw on a black background */
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_paint (cr);

    /* Change the transformation matrix */
    cairo_translate (cr, da.width / 2, da.height / 2);
    cairo_scale (cr, 30, -30);

    /* Determine the data points to calculate (ie. those in the clipping zone */
    cairo_device_to_user_distance (cr, &dx, &dy);
    cairo_clip_extents (cr, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
    cairo_set_line_width (cr, dx);

    /* Draws x and y axis */
    cairo_set_source_rgb (cr, 0.0, 1.0, 0.0);
    cairo_move_to (cr, -7.0, -7.0);
    cairo_line_to (cr, 7.0, -7.0);
    cairo_move_to (cr, -6.9, -7.0);
    cairo_line_to (cr, -7.0, 7.0);
    cairo_stroke (cr);
}

void helpbox()//I need to find a better way to do this, maybe pulling the text from a file but that could get sticky...not sure yet. figure out how to fix the cursor on the help screen also  
{
	GtkTextBuffer *buffer = gtk_text_buffer_new (NULL);
	gwidget.scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
	const gchar *text = {"Defaults exist for all Entry boxes. All radio buttons (circle selectors) are automatically set to their defaults.\nPrefiltering\n\nPrefiltering is an option, this is only needed for images that have multiple faces in them and it will split the picture into multiple pictures.\n\nPreprocess\n\nPreprocessing is REQUIRED prior to training.\n\nTraining\n\nTraining is only required if you do not use a pretrained model.\n\nGuess\n\nEvaluate\n\nExport\n\n"};
	gtk_text_buffer_set_text (buffer, text, -1);
    gwidget.help_view = gtk_text_view_new_with_buffer (buffer);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (gwidget.help_view), GTK_WRAP_WORD); 
    gtk_text_view_set_editable(GTK_TEXT_VIEW(gwidget.help_view), FALSE);
  //gtk_widget_set_sensitive(GTK_WIDGET(gwidget.help_view),FALSE);
  	add_context(style[3],gwidget.help_view);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (gwidget.scrolledwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); 
    gtk_container_add (GTK_CONTAINER (gwidget.scrolledwindow), gwidget.help_view);
    gtk_container_set_border_width (GTK_CONTAINER (gwidget.scrolledwindow), 5);
}

void preview_thumbnail(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    GdkRectangle da;            /* GtkDrawingArea size */
   // gdouble dx = 5.0, dy = 5.0; /* Pixels between each point */
    //gdouble i, clip_x1 = 0.0, clip_y1 = 0.0, clip_x2 = 0.0, clip_y2 = 0.0;

    /* Determine GtkDrawingArea dimensions */
    da.width = 30;
    da.height = 30;

    /* Draw on a black background */
    cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
    cairo_paint (cr);

    /* Change the transformation matrix */
    cairo_translate (cr, da.width / 2, da.height / 2);
    cairo_scale (cr, 30, -30);
}

static gboolean fill_progress ()
{
  fraction = gtk_progress_bar_get_fraction (GTK_PROGRESS_BAR (gwidget.progress_bar)); //Get the current progress
  fraction += 0.01;//Increase the bar by 10% each time this function is called
  gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR (gwidget.progress_bar), TRUE);//Fill in the bar with the new fraction
  gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR (gwidget.progress_bar), fraction);

  if (fraction < 1.0) //Ensures that the fraction stays below 1.0
  {
    return TRUE;
  }
  return FALSE;
}

void Age()
{
	location.age_gender = "age";
}

void Gender()
{
	location.age_gender = "gender";//change variable for gender and add parameters as needed to it
}

void setdefaults()
{
	if(location.model_type == NULL)
	{
		location.model_type = "";
	}

	if(location.crop == NULL)
	{
		location.crop = "False";
	}

	if(location.eval_data == NULL)
	{
		location.eval_data = "valid";
	}

	if(location.log == NULL)
	{
		location.log = "";
	}

	if(location.runonce == NULL)
	{
		location.runonce ="False";
	}

	if(location.optim == NULL)
	{
		location.optim = "";
	}

	if (location.face_detect_type == NULL)
	{
		location.face_detect_type = "";
	}

	if(location.age_gender == NULL)
	{
		location.age_gender = "gender";
	}
}

void prefilter() //create popup window that will have the progress bar in it and will show how many steps and a time until completion. 
{ //form command in here and run it in a loop, parsing the output to a file
	char *program = {"python filter_by_face.py "};
	char *detection_model = {" --face_detection_model "};
	char *detection_type = {" --face_detection_type "};
	char *filename = {" --filename "};
	char *target = {" --target "};
	char *pipe = {" > output.txt"};
	char command[1024];

	create_progress_window(0);
	setdefaults();

	strcpy(command, program);

	for(int i = 2; i < 5; i++)
	{
		if(location.filenames[i] != NULL)
		{
			switch(i)
			{
				case 2:strcat(command, filename);
				break;
				case 3:strcat(command, detection_model);
				break;
				case 4:strcat(command, target);
				break;
			}
			strcat(command, location.filenames[i]);
		}
	}

	if(strcmp(location.face_detect_type, "") != 0)
	{
		strcat(command, detection_type);
		strcat(command, location.face_detect_type);
	}

	strcat(command, pipe);
	puts(command);
	//system(command);
}

void process()//form command in here and run it in a loop, parsing the output to a file
{ 
	char *program = {"python preproc.py "};
	char *fold_dir = {" --fold_dir "};
	char *data_dir = {" --data_dir "};
	char *train_list = {" --train_list "};
	char *valid_list = {" --valid_list "};
	char *train_shards = {" --train_shards "};
	char *valid_shards = {" --valid_shards "};
	char *num_threads = {" --num_threads "};
	char *output_dir = {" --output_dir "};
	char *pipe = {" > output.txt"};
	char command[1024];

	create_progress_window(1);
	setdefaults();
	get_entry_text(gwidget.process_entries, location.process_entries, process_entry_size);
	
	strcpy(command, program);

	if(location.process_entries[0] != NULL)
	{
		strcat(command, train_shards);
		strcat(command, location.process_entries[0]);
	}

	if(location.process_entries[1] != NULL)
	{
		strcat(command, valid_shards);
		strcat(command, location.process_entries[1]);
	}

	if(location.process_entries[2] != NULL)
	{
		strcat(command, num_threads);
		strcat(command, location.process_entries[2]);
	}

	for(int i = 10; i < 15; i++)
	{
		if(location.filenames[i] != NULL)
		{
			switch(i)
			{
				case 10:strcat(command, data_dir);
				break;
				case 11:strcat(command, fold_dir);
				break;
				case 12:strcat(command, train_list);
				break;
				case 13:strcat(command, valid_list);
				break;
				case 14:strcat(command, output_dir);
				break;
			}
			strcat(command, location.filenames[i]);
		}
	}

	strcat(command, pipe);
	puts(command);
	//system(command);
}

void delay(int number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    int milli_seconds = 1000 * number_of_seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + milli_seconds) 
        ; 
} 

void train()//form command in here and run it in a loop, parsing the output to a file
{
	char *program = {"python train.py "};
	char *pre_checkpoint = {" --pre_checkpoint_path "};
	char *train_dir = {" --train_dir "};
	char *num_threads = {" --num_preprocess_threads "};
	char *image_size = {" --image_size "};
	char *eta = {" --eta "};
	char *eta_decay = {" --eta_decay_rate "};
	char *dropout = {" --pdrop "};
	char *log_or_not = {" --log_device_placement "};
	char *max_steps = {" --max_steps "};
	char *steps_per_decay = {" --steps_per_decay "};
	char *epochs = {" --epochs "};
	char *batch_size = {" --batch_size "};
	char *optim = {" --optim "};
	char *checkpoint = {" --checkpoint "};
	char *model_type = {" --model_type "};
	char *pre_model = {" --pre_model "};
	char *pipe = {" > output.txt"};
	char command[1024];
	
	strcpy(command, program);

	GThread *popup;
	popup = g_thread_new(NULL, (GThreadFunc)create_progress_window, (int *) 2);
	g_thread_join(popup);
	get_entry_text(gwidget.entries, location.train_entries, entry_len);
	setdefaults();

	if(strcmp(location.model_type, "") != 0)
	{
		strcat(command, model_type);
		strcat(command, location.model_type);
	}

	if(strcmp(location.log, "") != 0)
	{
		strcat(command, log_or_not);
		strcat(command, location.log);
	}

	if(strcmp(location.optim, "") != 0)
	{
		strcat(command, optim);
		strcat(command, location.optim);
	}

	for(int i = 17; i < 21; i++)
	{
		if(location.filenames[i] != NULL)
		{
			switch(i)
			{
				case 17:strcat(command, train_dir);
				break;
				case 18:strcat(command, pre_checkpoint);
				break;
				case 19:strcat(command, pre_model);
				break;
				case 20:strcat(command, checkpoint);
				break;
			}
			strcat(command, location.filenames[i]);
		}
	}

	for(int i = 0; i < entry_len; i++)
	{
		if(strcmp(location.train_entries[i], "") != 0)
		{
			switch(i)
			{
				case 0:strcat(command, batch_size);
				break;
				case 1:strcat(command, max_steps);
				break;
				case 2:strcat(command, image_size);
				break;
				case 3:strcat(command, eta);
				break;
				case 4:strcat(command, eta_decay);
				break;
				case 5:strcat(command, steps_per_decay);
				break;
				case 6:strcat(command, dropout);
				break;
				case 7:strcat(command, epochs);
				break;
				case 8:strcat(command, num_threads);
				break;
			}
			strcat(command, location.train_entries[i]);
		}
	}

	strcat(command, pipe);
	GThread *runthread;
	runthread = g_thread_new(NULL, (GThreadFunc) &runcommand, command);
	//g_thread_join(runthread);
	puts(command);
}
void runcommand(char command[])
{
	system(command);
}

void guess()//form command in here and run it in a loop, parsing the output to a file
{
	char *program = {"python guess.py "};
	char *model_dir = {" --model_dir "};
	char *class_type = {" --class_type "};
	char *filename = {" --filename "};
	char *device_id = {" --device_id "};
	char *target = {" --target "};
	char *checkpoint = {" --checkpoint "};
	char *model_type = {" --model_type "};
	char *requested_step = {" --requested_step "};
	char *single_look = {" --single_look "};
	char *face_detection_model = {" --face_detection_model "};
	char *face_detection_type = {" --face_detection_type "};
	char *pipe = {" > output.txt "};
	char command[1024];

	create_progress_window(3);
	setdefaults();
	get_entry_text(gwidget.guess_entries, location.guess_entries, guess_entry_size);
	strcpy(command, program);

	if(strcmp(location.age_gender, "") != 0)
	{
		strcat(command, class_type);
		strcat(command, location.age_gender);
	}
	
	if(strcmp(location.model_type, "") != 0)
	{
		strcat(command, model_type);
		strcat(command, location.model_type);
	}

	if(strcmp(location.crop, "False") != 0)
	{
		strcat(command, single_look);
		strcat(command, location.crop);
	}

	for(int i = 28; i < 32; i++)
	{
		if(location.filenames[i] != NULL)
		{
			switch(i)
			{
				case 28:strcat(command, model_dir);
				break;
				case 29:strcat(command, filename);
				break;
				case 30:strcat(command, checkpoint);
				break;
				case 31:strcat(command, face_detection_model);
				break;
			}
			strcat(command, location.filenames[i]);
		}
	}

	for(int i = 0; i < guess_entry_size; i++)
	{
		if(strcmp(location.guess_entries[i], "") != 0)
		{
			switch(i)
			{
				case 0:strcat(command, requested_step);
				break;
				case 1:strcat(command, device_id);
				break;
				case 2:strcat(command, face_detection_type);
				break;
				case 3:strcat(command, target);
				break;
			}
			strcat(command, location.guess_entries[i]);
		}
	}

	strcat(command, pipe);
	puts(command);
	system(command);
}

void eval()//form command in here and run it in a loop, parsing the output to a file
{
	char *program = {"python eval.py "};
	char *train_dir = {" --train_dir "};
	char *run_id = {" --run_id "};
	char *device_id = {" --device_id "};
	char *eval_dir = {" --eval_dir "};
	char *eval_data = {" --eval_data "};
	char *num_threads = {" --num_preprocess_threads "};
	char *interval = {" --eval_interval_secs "};
	char *examples = {" --num_examples "};
	char *image_size = {" --image_size "};
	char *batch_size = {" --batch_size "};
	char *checkpoint = {" --checkpoint "};
	char *model_type = {" --model_type "};
	char *run_once = {" --run_once "};
	char *requested_steps = {" --requested_step_seq "};
	char *pipe = {" > output.txt"};
	char command[1024];

	create_progress_window(4);
	setdefaults();
	get_entry_text(gwidget.eval_entries, location.eval_entries, eval_entry_size);

	strcpy(command, program);

	if(strcmp(location.model_type, "") != 0)
	{
		strcat(command, model_type);
		strcat(command, location.model_type);
	}

	if(strcmp(location.eval_data, "valid") != 0)
	{
		strcat(command, eval_data);
		strcat(command, location.eval_data);
	}

	if(strcmp(location.runonce, "False") != 0)
	{
		strcat(command, run_once);
		strcat(command, location.runonce);
	}

	if(location.filenames[44] != NULL)
	{
		strcat(command, train_dir);
		strcat(command, location.filenames[44]);
	}
	
	if(location.filenames[45] != NULL)
	{
		strcat(command, eval_dir);
		strcat(command, location.filenames[45]);
	}

	if(location.filenames[46] != NULL)
	{
		strcat(command, checkpoint);
		strcat(command, location.filenames[46]);
	}

		for(int i = 0; i < eval_entry_size; i++)
	{
		if(strcmp(location.eval_entries[i], "") != 0)
		{
			switch(i)
			{
				case 0:strcat(command, run_id);
				break;
				case 1:strcat(command, device_id);
				break;
				case 2:strcat(command, batch_size);
				break;
				case 3:strcat(command, image_size);
				break;
				case 4:strcat(command, requested_steps);
				break;
				case 5:strcat(command, interval);
				break;
				case 6:strcat(command, examples);
				break;
				case 7:strcat(command, num_threads);
				break;
			}
			strcat(command, location.eval_entries[i]);
		}
	}

	strcat(command, pipe);		
	puts(command);
	system(command);
}

void export() //form command in here and run it in a loop, parsing the output to a file
{
	char *program = {"python export.py "};
	char *checkpoint = {"--checkpoint "};
	char *class_type = {" --class_type "};
	char *model_dir = {" --filename "};
	char *model_version = {" --model_version "};
	char *output_dir = {" --output_dir "};
	char *model_type = {" --model_type "};
	char *requested_step = {" --requested_step "};
	char *pipe = {" > output.txt"};
	char command[1024];

	create_progress_window(5);
	setdefaults();
	get_entry_text(gwidget.export_entries, location.export_entries, export_entry_size);

	strcpy(command, program);

	if(strcmp(location.age_gender, "") != 0)
	{
		strcat(command, class_type);
		strcat(command, location.age_gender);
	}

	if(strcmp(location.model_type, "") != 0)
	{
		strcat(command, model_type);
		strcat(command, location.model_type);
	}

	if(location.filenames[54] != NULL)
	{
		strcat(command, checkpoint);
		strcat(command, location.filenames[54]);
	}

	if(location.filenames[55] != NULL)
	{
		strcat(command, output_dir);
		strcat(command, location.filenames[55]);
	}

	if(location.filenames[56] != NULL)
	{
		strcat(command, model_dir);
		strcat(command, location.filenames[56]);
	}
	
	if(location.export_entries[0] != NULL)
	{
		strcat(command, model_version);
		strcat(command, location.export_entries[0]);
	}

	if(location.export_entries[1] != NULL)
	{
		strcat(command, requested_step);
		strcat(command, location.export_entries[1]);
	}

	strcat(command, pipe);
	puts(command);
	system(command);
}

void bn()
{
	location.model_type = "bn";
}

void inception()
{
	location.model_type ="inception";
}

void default_model()
{
	location.model_type = "";
}

void singlecrop()
{
	location.crop = " True";
}
void multicrop()
{	
	location.crop = " False";
}

void eval_valid()
{
	location.eval_data = "valid";
}

void eval_train()
{
	location.eval_data = "train";
}

void addlog()
{
	location.log = "True";
}

void runonce()
{
	location.runonce = "True";
}

void adadelta()
{
	location.optim = "Adadelta";
}

void momentum()
{
	location.optim = "Momentum";
}

void cascade()
{
	location.face_detect_type = "cascade";
}

void yolo()
{
	location.face_detect_type = "yolo_tiny";
}

void dlib()
{
	location.face_detect_type = "dlib";
}

void prune()
{
	location.model_type = "prune";
}

gfloat f (gfloat x)
{
    return 0.03 * pow (x, 3);
}

/*void set_thumbnail(cairo_t *cr, gpointer user_data)
{
	GError * err = NULL;
	GdkPixbuf * imgpxb = gdk_pixbuf_new_from_file(,&err);
	gdk_cairo_set_source_pixbuf(cr, imgpxb, 0, 150);
	cairo_paint (cr);
}*/
void Exit()
{
	exit(EXIT_SUCCESS);
}
