#include "UIMessage.h"

UIMessage* UIMessage::theInstance = NULL;

UIMessage* UIMessage::getInstance()
{
	if (theInstance == NULL)
		theInstance = new UIMessage();
	return theInstance;
}

void UIMessage::release()
{
	if (theInstance != NULL)
		delete theInstance;
	theInstance = NULL;
}

UIMessage::UIMessage(void) {
	
}

UIMessage::~UIMessage(void) {
}

int UIMessage::DisplayError(string title, string message, int uType)
{
	int msgboxID = MessageBox(
		NULL,
		s2ws(message).c_str(),
		s2ws(title).c_str(),
		uType | MB_OK
	);

	return msgboxID;
}

void UIMessage::DisplayErrorAndQuit(string title, string message, int uType)
{
	// HideMainDialog();
	DisplayError(title, message, uType);

	SendMessage(UIMainWindow::getInstance()->getHandle() , WM_COMMAND, IDM_QUIT, NULL);
}

