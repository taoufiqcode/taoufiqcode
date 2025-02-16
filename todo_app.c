#include <stdio.h>
#include <gtk/gtk.h>

#define MAX_TASKS 100

struct task
{
    int id;
    char description[100];
    int isCompleted;
};

struct task taskList[MAX_TASKS];
int taskCount = 0;

// Declare function prototypes
void add_task_ui(GtkWidget *widget, gpointer data);
void delete_task_ui(GtkWidget *widget, gpointer data);
void mark_completed_ui(GtkWidget *widget, gpointer data);
void update_task_list_ui(GtkWidget *widget); // Add this line

// Declare the GTK widgets
GtkWidget *task_entry;
GtkWidget *task_listbox;

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    // Create the main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Task Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a vertical box container
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Create an entry for adding tasks
    task_entry = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(vbox), task_entry, FALSE, FALSE, 0);

    // Create buttons for adding, deleting, and marking tasks
    GtkWidget *add_button = gtk_button_new_with_label("Add Task");
    gtk_box_pack_start(GTK_BOX(vbox), add_button, FALSE, FALSE, 0);
    g_signal_connect(add_button, "clicked", G_CALLBACK(add_task_ui), NULL);

    GtkWidget *complete_button = gtk_button_new_with_label("Mark as Completed");
    gtk_box_pack_start(GTK_BOX(vbox), complete_button, FALSE, FALSE, 0);
    g_signal_connect(complete_button, "clicked", G_CALLBACK(mark_completed_ui), NULL);

    GtkWidget *delete_button = gtk_button_new_with_label("Delete Task");
    gtk_box_pack_start(GTK_BOX(vbox), delete_button, FALSE, FALSE, 0);
    g_signal_connect(delete_button, "clicked", G_CALLBACK(delete_task_ui), NULL);

    // Create a listbox to display tasks
    task_listbox = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(vbox), task_listbox, TRUE, TRUE, 0);

    // Show the window
    gtk_widget_show_all(window);

    // Start GTK main loop
    gtk_main();

    return 0;
}

void add_task_ui(GtkWidget *widget, gpointer data)
{
    const char *task_desc = gtk_entry_get_text(GTK_ENTRY(task_entry));

    // Add task to taskList directly
    if (task_desc && strlen(task_desc) > 0 && taskCount < MAX_TASKS)
    {
        taskList[taskCount].id = taskCount + 1;
        strcpy(taskList[taskCount].description, task_desc);
        taskList[taskCount].isCompleted = 0;
        taskCount++;

        // Clear the entry widget
        gtk_entry_set_text(GTK_ENTRY(task_entry), "");
    }

    // Refresh the task list display
    update_task_list_ui(NULL);
}

void delete_task_ui(GtkWidget *widget, gpointer data)
{
    if (taskCount > 0)
    {
        taskCount--; // Remove the last task
    }

    // Refresh the task list display
    update_task_list_ui(NULL);
}

void mark_completed_ui(GtkWidget *widget, gpointer data)
{
    if (taskCount > 0)
    {
        taskList[taskCount - 1].isCompleted = 1;
    }

    // Refresh the task list display
    update_task_list_ui(NULL);
}

void update_task_list_ui(GtkWidget *widget)
{
    // Clear the listbox before displaying new tasks
    GList *children, *iter;
    children = gtk_container_get_children(GTK_CONTAINER(task_listbox));
    for (iter = children; iter != NULL; iter = iter->next)
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    // Add each task to the listbox
    for (int i = 0; i < taskCount; i++)
    {
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(taskList[i].isCompleted ? g_strdup_printf("Task %d: %s [Completed]", taskList[i].id, taskList[i].description) : g_strdup_printf("Task %d: %s", taskList[i].id, taskList[i].description));

        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(task_listbox), row, -1);
        gtk_widget_show_all(row);
    }
}
