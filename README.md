# fve-arduino-victron
vytěžování fve do ohřevu vody.

Fungce: při SOC  98% se zapne ohřev,podle odběru proudu z baterie se buď přidá nebo ubere na výkonu.Dále se kontroluje každá fáze,aby nebyl překročen výkon střídače.Pokud výkon na fázi překročí limit střídače,na 2min. se daná fáze vypne.V průběhu 2.min vypnutí se kontroluje,zda se na dané fázi neobjeví spotřeba,pokud ano,vypnutí se resetuje(znovu se čeká 2.min).
při SOC 96% se vytěžování vypne.
při SOC 100% jede vytěžování naplno + kontrola fází.


arduino mega + ethernet shield

https://www.aliexpress.com/item/20000003997748.html?spm=a2g0o.productlist.0.0.5cc91133PYdiVt&algo_pvid=8afa9df4-92d6-4c9a-804b-b8f5d9ec807e&algo_exp_id=8afa9df4-92d6-4c9a-804b-b8f5d9ec807e-0&pdp_ext_f=%7B%22sku_id%22%3A%2230000000003970158%22%7D&pdp_npi=2%40dis%21CZK%21%21359.2%21%21%21%21%21%402100bdf116583940876852325e3ed6%2130000000003970158%21sea
