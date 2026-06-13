class HttpClient {
  static async getData(urlLink) {
    try {
      const response = await fetch(urlLink);

      if (!response.ok) {
        throw new Error(`Response status: ${response.status}`);
      }

      return response.json();
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }

  static async getResponse(urlLink) {
    const response = await fetch(urlLink, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ ping: true }),
    });

    return response.status;
  }

  static async sendData(urlLink, data) {
    try {
      const response = await fetch(urlLink, {
        method: "POST",
        headers: {
          // "Content-Type": "application/x-www-form-urlencoded",
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      });

      if (!response.ok) {
        throw new Error(`Response status: ${response.status}`);
      }

      return response.json();
    } catch (error) {
      console.error(error.message);
      throw error;
    }
  }
}

export default HttpClient;
