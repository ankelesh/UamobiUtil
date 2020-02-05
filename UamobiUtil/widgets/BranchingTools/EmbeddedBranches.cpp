#include "EmbeddedBranches.h"






namespace embeddedBranches
{
	const QString inventoryDesc
	(
		"[PlaceSelect]\n"
		"[ParentDocument]\n"
		"[InventoryParameters]\n"
		"[NormalScaning]\n"
		"<>\n"
		"<inv_add_item&session=%1&code=%2&qty=%3&show_code=%4 : barcode , control1 , show_code>\n"
		"*-5*"
		);
	const QString receiptDesc
	(
		"[PlaceSelect]\n"
		"[SelectItemFromList]\n"
		"%Supplier%\n"
		"[IdDependentSelect]\n"
		"%Order%\n"
		"[ReceiptParameters]\n"
		"[NormalScaning]\n"
		"<>\n"
		"*-6*"
	);
	const QString partInventoryDesc
		(
			"[PlaceSelect]\n"
			"[ParentDocument]\n"
			"[InventoryParameters]\n"
			"[BarcodeFilterSelect]\n"
			"[NormalScaning]\n"
			"<>\n"
			"<inv_add_item&session=%1&code=%2&qty=%3&show_code=%4 : barcode , control1 , show_code>\n"
			"*-5*"
		);
	const QString warehouseReceiptDesc
	(
		"[PlaceSelect]\n"
		"[SelectItemFromList]\n"
		"%Supplier%\n"
		"<rec_list_warehouses&session=%1&text=%2&hasorders=%3>\n"
		"[IdDependentSelect]\n"
		"%Order%\n"
		"[ReceiptParameters]\n"
		"[NormalScaning]\n"
		"<>\n"
		"*-6*"
	);
	const QString correctionReceiptDesc
		(
			"[PlaceSelect]\n"
			"[SelectItemFromList]\n"
			"%Supplier%\n"
			"[IdDependentSelect]\n"
			"%Order%\n"
			"[ReceiptParameters]\n"
			"[NormalScaning]\n"
			"<>\n"
			"*-6*"
		);
}