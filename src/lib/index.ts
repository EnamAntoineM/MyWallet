// place files you want to import through the `$lib` alias in this folder.
// Will add the objects declarations and the function to be used in the html file here

//Wallet object
//Transaction object
//get entity function
//update entity function
//delete entity function
//create entity function
//search entity function
//perma_del function


export type Wallet = {
    id: number;
    name: string;
    currency: string;
    source: string;
    initial_amount: number;
    balance: number;
    color: string;
    created_at: string;
    updated_at: string;
    is_active: boolean;
};

export type Transaction = {
    id: number;
    wallet_name: string;
    type: string;
    category: string;
    description: string;
    amount: number;
    related_wallet_name: string;
    recorded_at: string;
    updated_at: string;
    is_archived: any;
}
export async function refresh() {
    return { wallets: await get_wallets(), records: await get_records() }
}
//1-Create wallet
export async function create_wallet(to_create: Wallet) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_create);
    if (await window.saucer.exposed.create_wallet(intermediate)) {
		success_popup = "Wallet created successfully";
	} else {
		error_popup = "Failed to create wallet";
	}
    return {success_popup, error_popup};
}
//2-record transaction
export async function record_transaction(to_record: Transaction) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_record);
    if (await window.saucer.exposed.record_transaction(intermediate)) {
        success_popup = "Transaction recorded successfully";
	} else {
		error_popup = "Failed to record transaction";
    }
    return {success_popup, error_popup};
}
//3-record transfer
export async function record_transfer(to_record: Transaction) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_record);
    if (await window.saucer.exposed.record_transfer(intermediate)) {
        success_popup = "Transfer recorded successfully";
	} else {
		error_popup = "Failed to record transfer";
    }
    return {success_popup, error_popup};
}
//5-update record
export async function update_record(to_update: Transaction) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_update);
    if (await window.saucer.exposed.update_record(intermediate)) {
        success_popup = "Transaction updated successfully";
	} else {
		error_popup = "Failed to update transaction";
    }
    return {success_popup, error_popup};
}
//6-update wallet
export async function update_wallet(to_update: Wallet) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_update);
    if (await window.saucer.exposed.update_wallet(intermediate)) {
        success_popup = "Wallet updated successfully";
	} else {
		error_popup = "Failed to update wallet";
    }
    return {success_popup, error_popup};
}
//7-restore wallet
export async function restore_wallet(to_restore: Wallet) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_restore);
    if (await window.saucer.exposed.restore_wallet(intermediate)) {
        success_popup = "Wallet restored successfully";
	} else {
		error_popup = "Failed to restore wallet";
    }
    return {success_popup, error_popup};
}
//8-get wallets
export async function get_wallets() {
    let fetched_wallet: Wallet[] = JSON.parse(await window.saucer.exposed.get_wallets()) as Wallet[];
    return fetched_wallet;
}
//9-get records
export async function get_records() {
    let error_popup: string = "";
    let fetched_records: Transaction[] = JSON.parse(await window.saucer.exposed.get_records()) as Transaction[];
    return fetched_records;
}
//10-trash wallet
export async function trash_wallet(to_trash: Wallet) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_trash);
    if (await window.saucer.exposed.delete_wallet(intermediate)) {
        success_popup = "Wallet trashed successfully";
	} else {
		error_popup = "Failed to trash wallet";
    }
    return {success_popup, error_popup};
}
//11-delete wallet
export async function delete_wallet(to_delete: Wallet) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_delete);
    if (await window.saucer.exposed.delete_wallet_permanently(intermediate)) {
        success_popup = "Wallet deleted successfully";
	} else {
		error_popup = "Failed to delete wallet";
    }
    return {success_popup, error_popup};
}
//12-delete record
export async function delete_record(to_delete: Transaction) {
    let success_popup: string = "", error_popup: string = "";
    const intermediate = JSON.stringify(to_delete);
    if (await window.saucer.exposed.delete_record(intermediate)) {
        success_popup = "Record deleted successfully";
	} else {
		error_popup = "Failed to delete record";
    }
    return {success_popup, error_popup};
}