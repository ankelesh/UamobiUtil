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
		"<inv_new_doc&session=%1&date=%2&parent=%3&comment=%4>\n"
		"*-1*\n"
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
		"*-6*\n"
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
			"*-6*\n"
		);
	const QString printingDesc
	(
		"[PlaceSelect]\n"
		"[PrintingScaning]\n"
		"*-6*\n"
	);
}