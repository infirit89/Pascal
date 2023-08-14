class RequestQueryBuilder {

    constructor() {
        this.params = [];
    }

    add(key, value) {
        this.params.push(`${encodeURIComponent(key)}=${encodeURIComponent(value)}`);
    }

    build() {
        return this.params.join('&');
    }
}

class TestParams 
{
    constructor() 
    {
        this.testOption = 'cum';
    }
}

function Test() 
{
    const http = new XMLHttpRequest();
    const url = 'http://localhost:8080/test';
    const requestQueryBuilder = new RequestQueryBuilder();


    const testParams = new TestParams();
    testParams.testOption = 'cum2';

    requestQueryBuilder.add('json', JSON.stringify(testParams));

    http.open("POST", url);
    http.send(requestQueryBuilder.build());

    http.onreadystatechange = function() 
    {
        if(this.readyState == 4 && this.status == 200) 
        {
            console.log(http.response);
            console.log(http.responseText);
            console.log(http.responseType);
            console.log(http.responseURL);
            console.log(http.responseXML);
        }
    }

    // console.log("i cum");
}
