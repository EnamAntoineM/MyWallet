export {};

declare global {
	interface SaucerExposed {
		record_transaction:        (to_record: string) => Promise<boolean>;
		record_transfer:           (to_record: string) => Promise<boolean>;
		create_wallet:             (to_create: string) => Promise<boolean>;
		update_record:             (to_update: string) => Promise<boolean>;
		update_transfer:           (to_update: string) => Promise<boolean>;
		update_wallet:             (to_update: string) => Promise<boolean>;
		restore_wallet:			   (to_restore: string) =>Promise<boolean>;
		get_wallets:               ()  => Promise<string>;
		get_records:               ()  => Promise<string>;
		delete_record:             (to_delete: string) => Promise<boolean>;
		delete_wallet:             (to_delete: string) => Promise<boolean>;
		delete_wallet_permanently: (to_delete: string) => Promise<boolean>;
	}

	interface Saucer {
		exposed: SaucerExposed;
	}

	interface Window {
		saucer: Saucer;
	}
}
