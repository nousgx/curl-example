const express = require('express')
const app = express()
const port = 3000

app.use(express.json());

app.get('/', (req, res) => {
  res.send('Hello World!\n')
})

app.get('/delay/', (req, res) => {
  setTimeout(() => {
    console.log("Sending delay\n");
    res.send('Delaying...\n');
  }, 2000)
})

app.post('/', (req, res) => {
  console.log(req.body);
  res.send('Received a post at "/"');
})

app.listen(port, () => {
  console.log(`Example app listening at http://localhost:${port}`)
})