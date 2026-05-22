class LocalStorage {
  static setData(key, value) {
    localStorage.setItem(key, JSON.stringify(value));
    console.log("SetData");
  }

  static getData(key) {
    localStorage.getItem(JSON.stringify(key));
  }
}

export default LocalStorage;
