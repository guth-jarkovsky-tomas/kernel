// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <mm/heap.h>

// implementace bere rady z několik webů a odpovědí na stack overflow, ale implementace je naprosto moje (a také odpovídajícně chybná)

//struct na popisování bloku pameti v haldě 
//důležité je že tato struktura je v paměti NAD samotnou pamětí na kterou odkazuje, protože proč ne
//až na data_block úplně na spodku haldy, který nikam neodkazuje a nic nepopisuje

