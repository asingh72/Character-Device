
#include <linux/init.h>           
#include <linux/module.h>         
#include <linux/device.h>         
#include <linux/kernel.h>         
#include <linux/fs.h> 
#include<linux/slab.h>            
#include <asm/uaccess.h>
#include <linux/sched.h>
#define  DEVICE_NAME "process_list"    
#define  CLASS_NAME  "proc"        

MODULE_LICENSE("GPL");             

static int    majorNumber;                  
static struct class*  processClass  = NULL; 
static struct device* processDevice = NULL; 

static ssize_t dev_read(struct file *, char *, size_t, loff_t *);

static struct file_operations fops =
{
	.read = dev_read
};


static int __init char_init(void)
{
	majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
	if (majorNumber<0){
		printk(KERN_ALERT "Failed to register a major number\n");
	        return majorNumber;
   	}
	printk(KERN_INFO "Registered correctly with major number %d\n", majorNumber);
	processClass = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(processClass)){                
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to register device class\n");
		return PTR_ERR(processClass);          
	}
	processDevice = device_create(processClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   	if (IS_ERR(processDevice)){               
		class_destroy(processClass);           
		unregister_chrdev(majorNumber, DEVICE_NAME);
		printk(KERN_ALERT "Failed to create the device\n");
		return PTR_ERR(processDevice);
   	}
	return 0;
}


static void __exit char_exit(void)
{
	device_destroy(processClass, MKDEV(majorNumber, 0));     
   	class_unregister(processClass);                          
   	class_destroy(processClass);                             
   	unregister_chrdev(majorNumber, DEVICE_NAME);            
}




static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int error_count = 0;
	char * my1 = kmalloc(len,GFP_KERNEL);
	struct task_struct *task;
	for_each_process(task)	
	{	
		int d=task->state;
		char *cstate;
		if(d==0)
		cstate="TASK_RUNNING";
		else if(d==1)
		cstate="TASK_INTERRUPTIBLE";
		else if(d==2)
		cstate="TASK_UNINTERRUPTIBLE";
		else if(d==4)
		cstate="__TASK_STOPPED";
		else if (d==8)
		cstate="TASK_TRACED";
		else if (d==64)
		cstate="TASK_DEAD";
		else if (d==128)
		cstate="TASK_WAKEKILL";
		else if (d==128)
		cstate="TASK_WAKEKILL";
		else if (d==256)
		cstate="TASK_WAKING";
		else if (d==512)
		cstate="TASK_PARKED";
		else if (d==1024)
		cstate="TASK_NOLOAD";
		else if (d==2048)
		cstate="TASK_NEW";
		else if (d==4096)
		cstate="TASK_STATE_MAX";
		sprintf(my1+strlen(my1),"PID:%d PPID:%d CPU:%d STATE:%s\n", task->pid, task_ppid_nr(task), task_cpu(task), cstate);
	}

	error_count = copy_to_user(buffer, my1, strlen(my1));

}
module_init(char_init);
module_exit(char_exit);
