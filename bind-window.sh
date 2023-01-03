xprop | grep '_NET_WM_VISIBLE_NAME(UTF8_STRING)' | grep -oE '"(.*?)"' | tr -d '"'
