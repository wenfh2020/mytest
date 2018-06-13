package main

import "fmt"

type tagPersonInfo struct {
	strID      string
	strName    string
	strAddress string
}

func main() {
	fmt.Println("vim-go")

	var mapPersion map[string]tagPersonInfo
	mapPersion = make(map[string]tagPersonInfo)

	mapPersion["12345"] = tagPersonInfo{"12345", "jom", "Room12A"}
	mapPersion["1"] = tagPersonInfo{"1", "king", "Room12B"}

	tPerson, ok := mapPersion["123456"]
	if ok {
		fmt.Println("Found persion", tPerson.strName)
	} else {
		fmt.Println("Did not find persion")
	}

	//panic("error")
}
