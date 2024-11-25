TaskQueue::TaskQueue(){
	queueSemaphore = xSemaphoreCreateBinary();
	xSemaphoreGive(queueSemaphore);
}

TaskQueue::~TaskQueue(){}
