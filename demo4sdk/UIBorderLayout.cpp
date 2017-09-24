
#include "UI.h"

UIBorderLayout::UIBorderLayout() : UILayout()
{
	this->center = nullptr;
	this->left = nullptr;
	this->right = nullptr;
	this->top = nullptr;
	this->bottom = nullptr;
	this->margin = 5;
}

UIBorderLayout::~UIBorderLayout()
{
	if (this->center) {
		delete this->center;
	}
	if (this->left) {
		delete this->left;
	}
	if (this->right) {
		delete this->right;
	}
	if (this->top) {
		delete this->top;
	}
	if (this->bottom) {
		delete this->bottom;
	}
}

UIComponent* UIBorderLayout::setParentHWnd(HWND hParentWnd)
{
	UIComponent::setParentHWnd(hParentWnd);
	/*hWnd = CreateWindow(L"STATIC", L"",
		WS_CHILD | WS_VISIBLE,
		x, y, width, height, hParentWnd, 0, 0, 0);*/
	if (this->center) {
		this->center->setParentHWnd(hParentWnd);
	}
	if (this->left) {
		this->left->setParentHWnd(hParentWnd);
	}
	if (this->right) {
		this->right->setParentHWnd(hParentWnd);
	}
	if (this->top) {
		this->top->setParentHWnd(hParentWnd);
	}
	if (this->bottom) {
		this->bottom->setParentHWnd(hParentWnd);
	}
	return this;
}

UIBorderLayout* UIBorderLayout::setCenterComponent(UIComponent* component)
{
	this->center = component;
	return this;
}

UIBorderLayout* UIBorderLayout::setLeftComponent(UIComponent* component)
{
	this->left = component;
	return this;
}

UIBorderLayout* UIBorderLayout::setRightComponent(UIComponent* component)
{
	this->right = component;
	return this;
}

UIBorderLayout* UIBorderLayout::setTopComponent(UIComponent* component)
{
	this->top = component;
	return this;
}

UIBorderLayout* UIBorderLayout::setBottomComponent(UIComponent* component)
{
	this->bottom = component;
	return this;
}

UIBorderLayout* UIBorderLayout::setMargin(int margin)
{
	this->margin = margin;
	return this;
}

int UIBorderLayout::getMargin()
{
	return margin;
}

UIComponent* UIBorderLayout::layout() {
	UILayout::layout();
	int x = this->x;
	int y = this->y;
	int width = this->width;
	int height = this->height;

	int leftSize = 0, rightSize = 0, topSize = 0, bottomSize = 0;
	if (this->left) leftSize = 100;
	if (this->right) rightSize = 100;
	if (this->top) topSize = 100;
	if (this->bottom) bottomSize = 100;

	int leftSizeMargin = leftSize, rightSizeMargin = rightSize, topSizeMargin = topSize, bottomSizeMargin = bottomSize;
	if (this->left) leftSizeMargin += margin;
	if (this->right) rightSizeMargin += margin;
	if (this->top) topSizeMargin += margin;
	if (this->bottom) bottomSizeMargin += margin;

	if (this->center) this->center->setRect(x + leftSizeMargin, y + topSizeMargin, width - leftSizeMargin - rightSizeMargin, height - topSizeMargin - bottomSizeMargin)->layout();
	if (this->left) this->left->setRect(x, y + topSizeMargin, leftSize, height - topSizeMargin - bottomSizeMargin)->layout();
	if (this->right) this->right->setRect(x + width - rightSize, y + topSizeMargin, rightSize, height - topSizeMargin - bottomSizeMargin)->layout();
	if (this->top) this->top->setRect(x, y, width, topSize)->layout();
	if (this->bottom) this->bottom->setRect(x, y + height - bottomSize, width, bottomSize)->layout();
	return this;
}

void UIBorderLayout::messageMouseClick(int x, int y, UIMouseButton button)
{
	if (top && y < top->height) {
		top->messageMouseClick(x, y, button);
	}
	else if (bottom && y > bottom->y) {
		y -= bottom->y;
		bottom->messageMouseClick(x, y, button);
	}
	else if (left && x < left->width && y > left->y && y < left->y + left->height) {
		y -= left->y;
		left->messageMouseClick(x, y, button);
	}
	else if (right && x > right->x && y > right->y && y < right->y + right->height) {
		x -= right->x;
		y -= right->y;
		right->messageMouseClick(x, y, button);
	}
	else if (center && x > center->x && x < center->x + center->width && y > center->y && y < center->y + center->height) {
		x -= center->x;
		y -= center->y;
		center->messageMouseClick(x, y, button);
	}
}

UIComponent* UIBorderLayout::findByCode(int codeWnd)
{
	UIComponent* c = nullptr;
	if (!c && center) c = center->findByCode(codeWnd);
	if (!c && left) c = left->findByCode(codeWnd);
	if (!c && right) c = right->findByCode(codeWnd);
	if (!c && top) c = top->findByCode(codeWnd);
	if (!c && bottom) c = bottom->findByCode(codeWnd);
	return c;
}

UIComponent* UIBorderLayout::findByPoint(int x, int y)
{
	if (center && x > center->x && x < center->x + center->width && y > center->y && y < center->y + center->height) {
		x -= center->x;
		y -= center->y;
		return center->findByPoint(x, y);
	}
	else if (top && y < top->height) {
		return top->findByPoint(x, y);
	}
	else if (bottom && y > bottom->y) {
		y -= bottom->y;
		return bottom->findByPoint(x, y);
	}
	else if (left && x < left->width && y > left->y && y < left->y + left->height) {
		y -= left->y;
		return left->findByPoint(x, y);
	}
	else if (right && x > right->x && y > right->y && y < right->y + right->height) {
		x -= right->x;
		y -= right->y;
		return right->findByPoint(x, y);
	}
	return nullptr;
}